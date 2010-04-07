/* 功能：显示文本模型的视图
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


#ifndef TextBasedView_h
#define TextBasedView_h

#include <QWebView>
#include <QNetworkAccessManager>

namespace okular {
    class TextBasedView : public QWebView {
        Q_OBJECT
    public:
        TextBasedView(QWidget *p);
        void setEncoding(QString encoding);
        int loadFile(QString filename);
        void setManager(QNetworkAccessManager* manager);
    protected:
        virtual void keyPressEvent ( QKeyEvent * ev );
    signals:
        void gotoPage(bool gotoNextPage);
    };
}
#endif // TextBasedView_h
