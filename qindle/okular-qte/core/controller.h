/* 功能：控制器类
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

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString>
#include <QWidget>
#include <QStandardItemModel>
#include <QSqlDatabase>

#define DATAFILE "/usr/local/share/qindle/system.db"

namespace okular {
    class TextBasedView;
    class AbstractModel;
    class ImageBasedModel;
    class TextBasedModel;
    class ImageBasedView;

    class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();
    ~Controller();
    QWidget* getView(QWidget *p);
    enum ViewerType {ImageBased, TextBased, OCRText};
    int getCurrentPageNo();
    int getTotalPage();
    QStandardItemModel* getTOCModel();
    void mainwindowfinished();
    void setZoomFactor(qreal factor);
    void setRotate(int degree);
    bool addBookmark(QString name);

public slots:
    //For QFileDialog
    void openFile(QString filename);
    //multi use
    void gotoPage(int page);
    void gotoPage(QString pagename);
    void gotoPage(bool gotoNextPage);

    //manage bookmark
    void removeBookmark(QString name);
    void changeBookmarkName(QString oldname, QString newname);

private slots:
    void refreshView();
private:
    ImageBasedView* imageview;
    TextBasedView* textview;
    ImageBasedModel* imagemodel;
    TextBasedModel* textmodel;

    ViewerType m_type;
    AbstractModel* m_model;
    bool needTOC;
    uint FileNameHash;
    QSqlDatabase db;
    QStandardItemModel* getBookmark();
signals:
    void finished(int ret);
    void setDisplayType(int type);
};
}

#endif // CONTROLLER_H
