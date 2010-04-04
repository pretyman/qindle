/* 功能：操作PDF文件
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

#ifndef PDFModel_h
#define PDFModel_h

#include "ImageBasedModel.h"
extern "C"{
#include "pdfapp.h"
}

namespace okular {
class PDFModel : public ImageBasedModel {
public:
    PDFModel();
    ~PDFModel();

    //from AbstractModel
    virtual int open(QString filename);
    virtual void close();
    virtual int getTOC();
    virtual int getCurrentPageNo();
    virtual void setPageByNo(int page);
    virtual void setPageByName(QString name){};
    virtual QString getCurrentPageName(){return QString();};

    //from ImageBasedModel
    virtual int getTotalPage();
    virtual QImage getCurrentImage(Qt::AspectRatioMode mode);

private:
    void pdf_showpage(int loadpage, int drawpage);
    pdfapp_t* app;
    fz_error error;

};
}
#endif // PDFModel_h
