#ifndef DJVUModel_h
#define DJVUModel_h

#include "ImageBasedModel.h"
#include "kdjvu.h"

namespace okular{
class DJVUModel : public ImageBasedModel {
public:
    DJVUModel();
    ~DJVUModel();

    //from AbstractModel
    virtual int open(QString filename);
    virtual void close();
    virtual int getTOC(){return 0;};
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
};
}
#endif // DJVUModel_h
