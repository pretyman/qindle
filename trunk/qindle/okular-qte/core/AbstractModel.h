#ifndef AbstractModel_h
#define AbstractModel_h

#include <QString>

namespace okular {

class TOCModel;
class Metadata;
//This is the base class.
class AbstractModel {

public:

    virtual int open(QString filename)=0;
    virtual void close()=0;

    //The following "page" means a page of image-based file, or a file in text-based archive.
    virtual QString getCurrentPageName()=0;
    virtual int getCurrentPageNo()=0;
    virtual void setPageByNo(int page)=0;
    virtual void setPageByName(QString name)=0;

    TOCModel *m_TOCModel;
    QString Password;
    virtual int getTOC()=0;
};

}
#endif // AbstractModel_h
