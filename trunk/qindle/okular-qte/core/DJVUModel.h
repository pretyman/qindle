/* 功能：操作DJVU文件
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


#ifndef DJVUModel_h
#define DJVUModel_h

#include "ImageBasedModel.h"
#include "kdjvu.h"
#include <QDomElement>

namespace okular{
class DJVUModel : public ImageBasedModel {
public:
    DJVUModel();
    ~DJVUModel();

    //from AbstractModel
    virtual int open(QString filename);
    virtual void close();
    virtual int getTOC();
    virtual int getCurrentPageNo();
    virtual void setPageByNo(int page);
    virtual void setPageByName(QString name);
    virtual QString getCurrentPageName();
    //from ImageBasedModel
    virtual int getTotalPage();
    virtual QImage getCurrentImage(Qt::AspectRatioMode mode);

private:
    KDjVu* m_djvu;
    int currentPage;
    int totalPage;
    void recurseTOC(QDomElement currentelement, QStandardItem* currentitem);
};
}
#endif // DJVUModel_h
