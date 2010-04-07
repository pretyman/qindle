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

#include "CHMModel.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include "chmreply.h"
#include <QByteArray>
#include <string>
#include <QXmlStreamReader>
#include <QStandardItem>
#include <QXmlStreamAttributes>
#include <QTextCodec>

#define DATA_SIZE 256
#define DATA_CHUNK 256
using namespace okular;

int CHMModel::open(QString filename)
{
    m_file=chm_open(filename.toLocal8Bit().data());
    if(m_file==NULL)
        return -1;
    TOCResolved=false;
    codecName="utf8";
    return init();
}

void CHMModel::close()
{
    chm_close(m_file);
}

QNetworkReply* CHMModel::createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData)
{
    QNetworkReply* ret;
    //qWarning("%s", req.url().toString().toUtf8().data());
    if(req.url().scheme()=="file")
        ret=new CHMReply(this,req, op, m_file);
    else
        ret=QNetworkAccessManager::createRequest(op,req,outgoingData);
    return ret;
}

int CHMModel::init()
{
    chmUnitInfo cui;
    int i=chm_resolve_object(m_file, "/#SYSTEM",&cui);

    if(i==CHM_RESOLVE_FAILURE)
        return -1;
    char data[DATA_SIZE]="";
    QByteArray filename;
    chm_retrieve_object(m_file, &cui, (unsigned char *)data, 0, DATA_SIZE);
    //the following decodes topic file name from #system file.
    char* code;
    char* length;
    while(i<DATA_SIZE) {
        code=data+i;
        length=data+i+2;
        if (*code==0x02) {
            filename.append(data+i+4,(int)*length);
            break;
        } else {
            i=i+4+(int)*length;
        } ;
    }
    TopicName=QString(filename);
    filename.clear();

    //the following code doesn't work in all conditions.
    i=chm_resolve_object(m_file, "/#STRINGS",&cui);

    if(i==CHM_RESOLVE_FAILURE)
        return -1;
    chm_retrieve_object(m_file, &cui, (unsigned char *)data, 0, DATA_SIZE);
    //the following decodes toc file name from #strings file.
    length=data;
    while(i<3) {
        code=length;
        length=strchr(code+1,0);
        i++;
    };
    filename.append(code+1,length-code);
    //workaround
    if(filename.contains("hhc")) {
        TOCName="/"+QString(filename);
    } else {
        i=chm_resolve_object(m_file, "/toc.hhc",&cui);
        if(i==CHM_RESOLVE_SUCCESS)
            TOCName="/toc.hhc";
        else
            TOCName="";
    }

    return 0;
}
QString CHMModel::getCurrentPageName()
{
    return "/"+TopicName;
}

int CHMModel::getCurrentPageNo()
{
    int i=getTOC();
    if(i==0)
        return Spine.indexOf(TopicName);
    else return -1;
}

void CHMModel::setPageByNo(int page)
{
    int i=getTOC();
    if(i<0)
        return;
    if(page<0)
        page=0;
    else if(page>=Spine.count())
        page=Spine.count()-1;
    TopicName=Spine.at(page);
}

int CHMModel::getTotalPage()
{
    int i=getTOC();
    if(i==0)
        return Spine.count();
    else
        return -1;
}

void CHMModel::setPageByName(QString name)
{
    TopicName=name;
}

int CHMModel::getTOC()
{
    if(TOCResolved)
        return 0;
    chmUnitInfo cui;
    int i=chm_resolve_object(m_file, TOCName.toAscii().constData(),&cui);
    if(i==CHM_RESOLVE_FAILURE)
        return -1;
    QByteArray qdata(cui.length,0);
    chm_retrieve_object(m_file, &cui, (unsigned char *)qdata.data(), 0, cui.length);

    AbstractModel::initTOC();
    //the root item
    QStandardItem* currentitem=new QStandardItem(tr("Index"));
    QStandardItem *nameitem=0, *urlitem=0;
    m_TOCModel.appendRow(currentitem);
    //xmlreader report that the hhc is "not well formed", so we use a hand-made parser.

    //the following code is based on kchmviewer, and with some mods.
    /* Copyright (C) 2004-2007 by Georgy Yunaev, gyunaev@ulduzsoft.com       *
     * Portions Copyright (C) 2003  Razvan Cojocaru <razvanco@gmx.net>       */
    /*
     * FIXME: <OBJECT type="text/sitemap"><param name="Merge" value="hhaxref.chm::/HHOCX_c.hhc"></OBJECT>
     *  (from htmlhelp.chm)
    */

    //some SOB use gbk encoding, while others don't like it....
    QString src=QTextCodec::codecForName(codecName)->toUnicode(qdata);

    int pos = 0;
    bool in_object = false;

    // Split the HHC file by HTML tags
    int stringlen = src.length();

    while ( pos < stringlen && (pos = src.indexOf ('<', pos)) != -1 )
    {
        int i, j, word_end = 0;

        for ( i = ++pos; i < stringlen; i++ )
        {
            // If a " or ' is found, skip to the next one.
            if ( (src[i] == '"' || src[i] == '\'') )
            {
                // find where quote ends, either by another quote, or by '>' symbol (some people don't know HTML)
                int nextpos = src.indexOf (src[i], i+1);
                if ( nextpos == -1 	&& (nextpos = src.indexOf ('>', i+1)) == -1 )
                {
                    qWarning ("LCHMFileImpl::ParseHhcAndFillTree: corrupted TOC: %s", qPrintable( src.mid(i) ));
                    return false;
                }

                i =  nextpos;
            }
            else if ( src[i] == '>'  )
                break;
            else if ( !src[i].isLetterOrNumber() && src[i] != '/' && !word_end )
                word_end = i;
        }

        QString tagword, tag = src.mid (pos, i - pos);

        if ( word_end )
            tagword = src.mid (pos, word_end - pos).toLower();
        else
            tagword = tag.toLower();

        //qDebug ("tag: '%s', tagword: '%s'\n", qPrintable( tag ), qPrintable( tagword ) );

        // <OBJECT type="text/sitemap"> - a topic entry
        if ( tagword == "object" && tag.indexOf ("text/sitemap", 0, Qt::CaseInsensitive ) != -1 ) {
            in_object = true;
            nameitem=new QStandardItem();
            urlitem=new QStandardItem();
        }
        else if ( tagword == "/object" && in_object )
        {
            // a topic entry closed. Add a tree item
            if ( !nameitem->text().isEmpty() )
            {
                j=currentitem->rowCount();
                currentitem->setChild(j,0,nameitem);
                currentitem->setChild(j,1,urlitem);
                Spine.append(urlitem->text());
            }
            else if ( !urlitem->text().isEmpty() )
                    qWarning ("LCHMFileImpl::ParseAndFillTopicsTree: <object> tag with url is parsed, but name is empty.");
            else
                qWarning ("LCHMFileImpl::ParseAndFillTopicsTree: <object> tag is parsed, but both name and url are empty.");

            in_object = false;
        }
        else if ( tagword == "param" && in_object )
        {
            // <param name="Name" value="First Page">
            int offset; // strlen("param ")
            QString name_pattern = "name=", value_pattern = "value=";
            QString pname, pvalue;

            if ( (offset = tag.indexOf (name_pattern, 0, Qt::CaseInsensitive )) == -1 )
                qFatal ("LCHMFileImpl::ParseAndFillTopicsTree: bad <param> tag '%s': no name=\n", qPrintable( tag ));

            // offset+5 skips 'name='
            offset = findStringInQuotes (tag, offset + name_pattern.length(), pname, TRUE);
            pname = pname.toLower();

            if ( (offset = tag.indexOf(value_pattern, offset, Qt::CaseInsensitive )) == -1 )
                qFatal ("LCHMFileImpl::ParseAndFillTopicsTree: bad <param> tag '%s': no value=\n", qPrintable( tag ));

            // offset+6 skips 'value='
            findStringInQuotes (tag, offset + value_pattern.length(), pvalue, FALSE);

            //qDebug ("<param>: name '%s', value '%s'", qPrintable( pname ), qPrintable( pvalue ));

            if ( pname == "name" )
            {
                // Some help files contain duplicate names, where the second name is empty. Work it around by keeping the first one
                if ( !pvalue.isEmpty() )
                    nameitem->setText(pvalue);
            }
            else if ( pname == "local" )
            {
                urlitem->setText(pvalue);
            }
        }
        else if ( tagword == "ul" ) // increase indent level
        {
            if(nameitem)
                currentitem=nameitem;
        }
        else if ( tagword == "/ul" ) // decrease indent level
        {
            currentitem=currentitem->parent();
        }

        pos = i;
    }

    TOCResolved=true;
    return 0;
}

//this function is also stealed from kchmviewer.
inline int CHMModel::findStringInQuotes (const QString& tag, int offset, QString& value, bool firstquote)
{
        int qbegin = tag.indexOf ('"', offset);

        if ( qbegin == -1 )
                qFatal ("LCHMFileImpl::findStringInQuotes: cannot find first quote in <param> tag: '%s'", qPrintable( tag ));

        int qend = firstquote ? tag.indexOf ('"', qbegin + 1) : tag.lastIndexOf ('"');

        if ( qend == -1 || qend <= qbegin )
                qFatal ("LCHMFileImpl::findStringInQuotes: cannot find last quote in <param> tag: '%s'", qPrintable( tag ));

        // If we do not need to decode HTML entities, just return.
                value = tag.mid (qbegin + 1, qend - qbegin - 1);

        return qend + 1;
}
