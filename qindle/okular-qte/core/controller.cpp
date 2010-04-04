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

#include "controller.h"
#include <QFileInfo>
#include "DJVUModel.h"
#include "PDFModel.h"
#include "CHMModel.h"
#include "EPUBModel.h"
#include "ImageBasedView.h"
#include "TextBasedView.h"
#include <QSqlError>
#include <QSqlQuery>

using namespace okular;
Controller::Controller()
{
    imageview=0;
    textview=0;
    imagemodel=0;
    textmodel=0;
    m_model=0;
    needTOC=true;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DATAFILE);
    db.open();
}

Controller::~Controller()
{
    if(imageview) {
        imageview->close();
        delete imageview;
    } else if(textview) {
        textview->close();
        delete textview;
    }
    if(imagemodel) {
        imagemodel->close();
        delete imagemodel;
    } else if(textmodel) {
        textmodel->close();
        delete textmodel;
    }

    db.close();
}

void Controller::openFile(QString filename)
{
    QFileInfo m_file(filename);
    QString suf=m_file.suffix().toLower();
    int ret;
    if(suf=="djvu") {
        this->m_type=ImageBased;
        DJVUModel* djvumodel=new DJVUModel();
        imagemodel=djvumodel;
        m_model=djvumodel;
    } else if(suf=="pdf") {
        this->m_type=ImageBased;
        PDFModel* pdfmodel=new PDFModel();
        imagemodel=pdfmodel;
        m_model=pdfmodel;
    } else if(suf=="chm") {
        this->m_type=TextBased;
        CHMModel* chmmodel=new CHMModel();
        textmodel=chmmodel;
        m_model=chmmodel;
    } else if(suf=="epub") {
        this->m_type=TextBased;
        EPUBModel* epubmodel=new EPUBModel();
        textmodel=epubmodel;
        m_model=epubmodel;
    }
    ret=m_model->open(filename);
    FileNameHash=qHash(m_file.fileName());
    emit(finished(ret));
}

QWidget* Controller::getView(QWidget *p)
{
    if(this->m_type==ImageBased) {
        imageview=new ImageBasedView(p);
        connect(imageview,SIGNAL(gotoPage(bool)), this, SLOT(gotoPage(bool)));
        return qobject_cast<QWidget*>(imageview);
    }
     else if(m_type==TextBased) {
         textview=new TextBasedView(p);
         connect(textview,SIGNAL(gotoPage(bool)), this, SLOT(gotoPage(bool)));
         textview->setManager(textmodel);
        return qobject_cast<QWidget*>(textview);
    };
    return NULL;
}

void Controller::gotoPage(int page)
{
    m_model->setPageByNo(page);
    refreshView();
}
void Controller::gotoPage(QString pagename)
{
    m_model->setPageByName(pagename);
    refreshView();
}

void Controller::gotoPage(bool gotoNextPage)
{
    int page=getCurrentPageNo();
    if(page < getTotalPage() && gotoNextPage)
        gotoPage(page+1);
    else if(page >1 && !(gotoNextPage))
        gotoPage(page-1);
}

void Controller::refreshView()
{
    if(this->m_type==ImageBased) {
        imagemodel->ViewSize=imageview->backSize;
        QImage myimage=imagemodel->getCurrentImage(imageview->scalemode);
        imageview->loadImage(myimage);
    } else {
        QString pageurl=textmodel->getCurrentPageName();
        textview->loadFile(pageurl);
    }
}

int Controller::getCurrentPageNo()
{
    return m_model->getCurrentPageNo();
}

int Controller::getTotalPage()
{
    return m_model->getTotalPage();
}

QStandardItemModel* Controller::getTOCModel()
{
    if(needTOC) {
        m_model->getTOC();
        needTOC=false;
    };

    return this->getBookmark();
}

void Controller::mainwindowfinished()
{
    if(this->m_type==ImageBased) {
        imageview->updateSize();
        connect(this,SIGNAL(setDisplayType(int)), imageview, SLOT(setDisplayType(int)));
        connect(imageview, SIGNAL(refresh()), this, SLOT(refreshView()));
    }
    gotoPage(0);
}

void Controller::setZoomFactor(qreal factor)
{
    if(this->m_type==ImageBased) {
        imageview->setZoomFactor(factor);
        this->refreshView();
    } else
        textview->setZoomFactor(factor);
}

void Controller::setRotate(int degree)
{
    if(this->m_type==ImageBased) {
        imagemodel->pageRotate=degree;
        this->refreshView();
    };
}

QStandardItemModel* Controller::getBookmark()
{
    if(!db.isOpen())
        return &m_model->m_TOCModel;

    QSqlQuery query(db);
    query.prepare("SELECT name, pagenumber, pagename FROM bookmark WHERE hash = ?");
    query.addBindValue(QString::number(FileNameHash));
    query.exec();
    if(!query.size())
        return &m_model->m_TOCModel;

    QStandardItem *rootitem, *nameitem, *pageitem, *urlitem;
    int i;
    rootitem=new QStandardItem(tr("Bookmarks"));
    while(query.next()) {
        i=rootitem->rowCount();
        nameitem=new QStandardItem(query.value(0).toString());
        urlitem=new QStandardItem(query.value(2).toString());
        pageitem=new QStandardItem(query.value(1).toString());
        rootitem->setChild(i,0,nameitem);
        rootitem->setChild(i,1,urlitem);
        rootitem->setChild(i,2,pageitem);
    }

    QList<QStandardItem *> list=m_model->m_TOCModel.findItems(tr("Bookmarks"),Qt::MatchFixedString);
    if(list.count()) {
        nameitem=list.at(0);
        pageitem=m_model->m_TOCModel.takeItem(nameitem->row(), nameitem->column());
        delete pageitem;
    }
    m_model->m_TOCModel.setItem(1,rootitem);

    return &m_model->m_TOCModel;
}

bool Controller::addBookmark(QString name)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO bookmark (hash, name, pagenumber, pagename) VALUES (:hash, :name, :pagenumber, :pagename)");
    query.bindValue(":hash", FileNameHash);
    query.bindValue(":name", name);
    query.bindValue(":pagenumber", m_model->getCurrentPageNo());
    query.bindValue(":pagename", m_model->getCurrentPageName());
    return query.exec();
}

void Controller::removeBookmark(QString name)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM bookmark WHERE hash = :hash AND name = :name");
    query.bindValue(":hash", FileNameHash);
    query.bindValue(":name", name);
    query.exec();
    return;
}

void Controller::changeBookmarkName(QString oldname, QString newname)
{
    QSqlQuery query(db);
    query.prepare("UPDATE FROM bookmark SET name = :newname WHERE hash = :hash AND name = :name");
    query.bindValue(":hash", FileNameHash);
    query.bindValue(":name", oldname);
    query.bindValue(":newname", newname);
    query.exec();
    return;
}
