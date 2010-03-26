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
    virtual int getTOC(){return 0;};
    virtual int init();
protected:
    virtual QNetworkReply *	createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0 );
private:
    struct chmFile* m_file;
    QString TOCName;
    QString TopicName;

};
}
#endif // CHMModel_h
