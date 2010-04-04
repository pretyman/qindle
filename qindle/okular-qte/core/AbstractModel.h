/* 功能：模型的抽象类，任何模型均由此派生
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

#ifndef AbstractModel_h
#define AbstractModel_h

#include <QString>
#include <QStandardItemModel>

namespace okular {

class TOCModel;
class Metadata;
//This is the base class.
class AbstractModel {

public:

    virtual int open(QString filename)=0;
    virtual void close()=0;

    //The following "page" means a image in image-based file, or a file in text-based archive.
    virtual QString getCurrentPageName()=0;
    virtual int getCurrentPageNo()=0;
    virtual void setPageByNo(int page)=0;
    virtual void setPageByName(QString name)=0;
    virtual int getTotalPage()=0;
    QString Password;
    virtual int getTOC()=0;
    QStandardItemModel m_TOCModel;
protected:
    void initTOC();
};

}
#endif // AbstractModel_h
