/* 功能：基于图像的模型
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

#ifndef ImageBasedModel_h
#define ImageBasedModel_h
#include <QImage>
#include <QSize>
#include "AbstractModel.h"

namespace okular{
class ImageBasedModel : public AbstractModel  {

public:
    //This desides how to scale image if document and viewpoint don't have the same width/height ratio.
//    enum ImageMode { FixedWidth, FixedHeight ,FixedRect};


    virtual QImage getCurrentImage(Qt::AspectRatioMode mode)=0;

    int pageRotate;
    QSize ViewSize;
protected:
    QSize DocumentSize;

};
}
#endif // ImageBasedModel_h
