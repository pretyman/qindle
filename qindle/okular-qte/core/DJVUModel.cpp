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

#include "DJVUModel.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>

using namespace okular;

DJVUModel::DJVUModel()
{
    m_djvu=new KDjVu();
}

DJVUModel::~DJVUModel()
{
    delete m_djvu;
}

int DJVUModel::open(QString filename)
{
    if(m_djvu->openFile(filename)) {
        QVector<KDjVu::Page*> pages=m_djvu->pages();
        totalPage=pages.size();
        pageRotate=0;
        currentPage=0;
        return 0;
    };
    return -1;
}

void DJVUModel::close()
{
    m_djvu->closeFile();
}

void DJVUModel::setPageByNo(int page)
{
    //currentPage ranges from 0, but viewer's page ranges from 1.
    currentPage=page-1;
    if (currentPage<0)
        currentPage=0;
    else if(currentPage>=totalPage)
        currentPage=totalPage-1;

    KDjVu::Page* curpage=m_djvu->pages().at(currentPage);
    DocumentSize.setHeight(curpage->height());
    DocumentSize.setWidth(curpage->width());

}

QImage DJVUModel::getCurrentImage(Qt::AspectRatioMode mode)
{
    QSize actsize=this->DocumentSize;
    actsize.scale(this->ViewSize,mode);

    return m_djvu->image(this->currentPage,actsize.width(),actsize.height(),0);
}

int DJVUModel::getCurrentPageNo()
{
    return this->currentPage+1;
}

QString DJVUModel::getCurrentPageName()
{
    return QString();
}

void DJVUModel::setPageByName(QString name)
{
    int page=m_djvu->pageNumber(name);
    //that function needs a hack, see above.
    this->setPageByNo(page+1);
}

int DJVUModel::getTotalPage()
{
   return totalPage;
}

int DJVUModel::getTOC()
{
    AbstractModel::initTOC();
    //the root item
    QStandardItem* currentitem=new QStandardItem(QObject::tr("Index"));
    m_TOCModel.appendRow(currentitem);
    QDomElement item=this->m_djvu->documentBookmarks()->documentElement();

    recurseTOC(item, currentitem);
    return 0;
}

void DJVUModel::recurseTOC(QDomElement currentelement, QStandardItem *currentitem)
{
    QStandardItem *nameitem, *pageitem, *urlitem;
    QDomNodeList childlist;
    QDomNode child;
    int j;
    if(currentelement.hasAttribute("title")) {
        nameitem=new QStandardItem(currentelement.attribute("title"));
        urlitem=new QStandardItem(currentelement.attribute("PageName",""));
        pageitem=new QStandardItem(currentelement.attribute("PageNumber",""));
        j=currentitem->rowCount();
        currentitem->setChild(j,0,nameitem);
        currentitem->setChild(j,1,urlitem);
        currentitem->setChild(j,2,pageitem);
    }
    if(currentelement.hasChildNodes()) {
        childlist=currentelement.childNodes();
        for(j=0;j<childlist.count();j++) {
            child=childlist.at(j);
            recurseTOC(child.toElement(), nameitem);
        };
    }
    return;
}
