/* 功能：以文本为基础的模型
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

#ifndef TextBasedModel_h
#define TextBasedModel_h

#include "AbstractModel.h"
#include <QNetworkAccessManager>
#include <QString>

namespace okular{
class TextBasedModel : public QNetworkAccessManager, public AbstractModel {
public:
    //Page 0 is the topic page.
protected:
    virtual QNetworkReply *	createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0 )=0;

    virtual int init()=0;
    //This is the spine
    QStringList Spine;
};
}
#endif // TextBasedModel_h
