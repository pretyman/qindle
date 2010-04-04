/* 功能：操作EPUB文件
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

#ifndef EPUBModel_h
#define EPUBModel_h

#include "TextBasedModel.h"
#include "quazip.h"
#include <QHash>
#include <QStringList>


namespace okular {
class EPUBModel : public TextBasedModel {
public:
    EPUBModel();
    ~EPUBModel();

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
protected:
    virtual QNetworkReply *	createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0 );
    virtual int init();
private:
    QuaZip* m_zip;
    //This is the manifest(id,href)
    QHash<QString, QString> Manifest;
    QString opfpath;
    QString tocID;
    //this model is based on page number.
    int currentPage;
};
}
#endif // EPUBModel_h
