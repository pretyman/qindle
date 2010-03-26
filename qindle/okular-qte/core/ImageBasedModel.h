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

    virtual int getTotalPage()=0;
    virtual QImage getCurrentImage(Qt::AspectRatioMode mode)=0;

    int pageRotate;
    QSize ViewSize;
protected:
    QSize DocumentSize;

};
}
#endif // ImageBasedModel_h
