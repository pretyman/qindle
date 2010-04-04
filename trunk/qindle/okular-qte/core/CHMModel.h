/* 功能：操作CHM文件
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

#ifndef CHMModel_h
#define CHMModel_h

#include "TextBasedModel.h"
#include "chm_lib.h"

namespace okular{
class CHMModel : public TextBasedModel {

public:
    //from AbstractModel
    virtual int open(QString filename);
    virtual void close();

    //The following "page" means a page of image-based file, or a file in text-based archive.
    virtual QString getCurrentPageName();
    virtual int getCurrentPageNo();
    virtual void setPageByNo(int page);
    virtual void setPageByName(QString name);
    virtual int getTOC();
    virtual int getTotalPage();
    virtual int init();
protected:
    virtual QNetworkReply *	createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0 );
private:
    struct chmFile* m_file;
    QString TOCName;
    //this model is based on page name.
    QString TopicName;
    int findStringInQuotes (const QString& tag, int offset, QString& value, bool firstquote);
    bool TOCResolved;
};
}
#endif // CHMModel_h
