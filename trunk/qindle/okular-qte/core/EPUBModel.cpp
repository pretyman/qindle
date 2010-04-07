/*
 * Copyright (C) 2010 Li Miao <lm3783@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "EPUBModel.h"
#include "epubreply.h"
#include <QXmlStreamReader>
#include <QStandardItem>

using namespace okular;

EPUBModel::EPUBModel()
{
    m_zip=new QuaZip();
    currentPage=0;
}

EPUBModel::~EPUBModel()
{

    delete m_zip;
}

int EPUBModel::open(QString filename)
{
    m_zip->setZipName(filename);

    if(m_zip->open(QuaZip::mdUnzip,NULL)) {
        return init();
    } else
        qWarning("testRead(): zip.open(): %d", m_zip->getZipError());
    return -1;
}

void EPUBModel::close()
{
    m_zip->close();
}

QNetworkReply* EPUBModel::createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData)
{
    QNetworkReply* ret;
    qWarning("%s", req.url().toString().toAscii().data());
    if(req.url().scheme()=="file")
        ret=new EPUBReply(this,req,op,m_zip, opfpath);
    else
        ret=QNetworkAccessManager::createRequest(op,req,outgoingData);
    return ret;
}

QString EPUBModel::getCurrentPageName()
{
    //qwebpage's url must begin with '/'
    return "/"+Manifest.value(Spine.at(currentPage));
}

int EPUBModel::getCurrentPageNo()
{
    return currentPage;
}

void EPUBModel::setPageByName(QString name)
{
    QString id=Manifest.key(name);
    currentPage=Spine.indexOf(id);
}

void EPUBModel::setPageByNo(int page)
{
    if(page<0)
        page=0;
    else if(page>=Spine.count())
        page=Spine.count()-1;
    currentPage=page;
}

int EPUBModel::getTotalPage()
{
    return Spine.count();
}

int EPUBModel::init()
{
    //get opf file
    if(!(m_zip->setCurrentFile("META-INF/container.xml")))
        return -1;
    QuaZipFile ocffile(m_zip);
    ocffile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString opfname;
    QXmlStreamReader ocfreader(&ocffile);
    for(;!(ocfreader.atEnd());ocfreader.readNextStartElement()){
        if(ocfreader.name()=="rootfile") {
            opfname=ocfreader.attributes().value(QString(), "full-path").toString();
            break;
        }
    }
    ocfreader.clear();
    ocffile.close();
    if(opfname.isNull())
        return -1;

    if(opfname.contains("/"))
        opfpath=opfname.left(opfname.lastIndexOf("/")+1);
    else
        opfpath="";

    //parse opf file
    if(!(m_zip->setCurrentFile(opfname)))
        return -1;
    ocffile.open(QIODevice::ReadOnly | QIODevice::Text);
    //Some file have a "xml 1.1" heading, which streamreader doesn't like, so eat it.
    //qWarning("%s",ocffile.readLine(80).data());
    ocffile.readLine(255);
    ocfreader.setDevice(&ocffile);
    for(;!(ocfreader.atEnd());ocfreader.readNextStartElement()){
        //currently we do not handle metadata
        if(ocfreader.isEndElement())
            continue;
        //this is a ugly method. directly access elements.
        if(ocfreader.name()=="item")
            Manifest.insert(ocfreader.attributes().value(QString(), "id").toString(), ocfreader.attributes().value(QString(), "href").toString());
        else if(ocfreader.name()=="spine")
            tocID=ocfreader.attributes().value(QString(), "toc").toString();
        else if(ocfreader.name()=="itemref")
            Spine.append(ocfreader.attributes().value(QString(), "idref").toString());

    }
    ocfreader.clear();
    ocffile.close();

    return 0;
}

int EPUBModel::getTOC()
{
    QString tocfile=Manifest.value(tocID);
    if(!(m_zip->setCurrentFile(opfpath+tocfile)))
        return -1;
    QuaZipFile mytoc(m_zip);
    if(!(mytoc.open(QIODevice::ReadOnly))) {
        qWarning("Error number %d", mytoc.getZipError());
        return -1;
    }
    AbstractModel::initTOC();
    //the root item
    QStandardItem* currentitem=new QStandardItem(tr("Index"));
    QStandardItem* newitem;
    QStandardItem* tempitem;
    int i=0;
    bool inNav=false;
    m_TOCModel.appendRow(currentitem);
    QXmlStreamReader reader(&mytoc);
    for(;!(reader.atEnd());reader.readNextStartElement()){
        //some document have more things than navmap.
        if(reader.isStartElement() && reader.name()=="navMap")
            inNav=true;
        if(reader.isStartElement() && inNav) {
            if(reader.name()=="navPoint") {
                newitem=new QStandardItem();
                tempitem=new QStandardItem();
                i=currentitem->rowCount();
                currentitem->setChild(i,0,newitem);
                currentitem->setChild(i,1,tempitem);
                currentitem=newitem;
            } else if(reader.name()=="text") {
                newitem->setText(reader.readElementText());
            } else if(reader.name()=="content") {
                tempitem->setText(reader.attributes().value(QString(), "src").toString());
            }
        } else if((reader.isEndElement()) && (reader.name()=="navPoint")) {
            currentitem=currentitem->parent();
        } else if(reader.isEndElement() && reader.name()=="navMap")
            inNav=false;
    }
    reader.clear();
    mytoc.close();
    return 0;
}
