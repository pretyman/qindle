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
    virtual int getTOC(){return 0;};
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
