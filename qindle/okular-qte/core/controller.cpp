#include "controller.h"
#include <QFileInfo>
#include "DJVUModel.h"
#include "PDFModel.h"
#include "CHMModel.h"
#include "EPUBModel.h"
#include "ImageBasedView.h"
#include "TextBasedView.h"

using namespace okular;
Controller::Controller()
{
}

Controller::~Controller()
{
    if(m_type==ImageBased) {
        imageview->close();
        delete imageview;
    }
     else if(m_type==TextBased) {
        textview->close();
        delete textview;
    };
    m_model->data()->close();
    m_model->clear();
    delete m_model;

}

void Controller::openFile(QString filename)
{
    QFileInfo m_file(filename);
    QString suf=m_file.suffix().toLower();
    if(suf=="djvu") {
        this->m_type=ImageBased;
        DJVUModel* djvumodel=new DJVUModel();
        m_model=new QSharedPointer<AbstractModel>(djvumodel);
        m_model->data()->open(filename);
    } else if(suf=="pdf") {
        this->m_type=ImageBased;
        PDFModel* pdfmodel=new PDFModel();
        m_model=new QSharedPointer<AbstractModel>(pdfmodel);
        m_model->data()->open(filename);
    } else if(suf=="chm") {
        this->m_type=TextBased;
        CHMModel* chmmodel=new CHMModel();
        m_model=new QSharedPointer<AbstractModel>(chmmodel);
        m_model->data()->open(filename);
    } else if(suf=="epub") {
        this->m_type=TextBased;
        EPUBModel* epubmodel=new EPUBModel();
        m_model=new QSharedPointer<AbstractModel>(epubmodel);
        m_model->data()->open(filename);
    }
}

QWidget* Controller::getView(QWidget *p)
{
    if(this->m_type==ImageBased) {
        imageview=new ImageBasedView(p);
        return qobject_cast<QWidget*>(imageview);
    }
     else if(m_type==TextBased) {
         textview=new TextBasedView(p);
         textview->setManager(m_model->dynamicCast<TextBasedModel>().data());
        return qobject_cast<QWidget*>(textview);
    };
    return NULL;
}

void Controller::gotoPage(int page)
{
    if(this->m_type==ImageBased) {
        ImageBasedModel* mymodel=m_model->dynamicCast<ImageBasedModel>().data();
        mymodel->setPageByNo(page);
        mymodel->ViewSize=imageview->size();
        QImage myimage=mymodel->getCurrentImage(Qt::KeepAspectRatioByExpanding);

        imageview->loadImage(myimage);
    } else {
        TextBasedModel* mymodel=m_model->dynamicCast<TextBasedModel>().data();
        mymodel->setPageByNo(page);
        QString pageurl=mymodel->getCurrentPageName();
        textview->loadFile(pageurl);
    }
}

int Controller::getCurrentPageNo()
{
        return m_model->data()->getCurrentPageNo();
}

