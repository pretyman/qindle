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
    virtual int getTOC(){return 0;};

protected:
    virtual QNetworkReply *	createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0 );
    virtual int init();
private:
    QuaZip* m_zip;
    //This is the manifest(id,href)
    QHash<QString, QString> Manifest;
    //This is the spine
    QStringList Spine;
    QString opfpath;
    QString tocID;
};
}
#endif // EPUBModel_h
