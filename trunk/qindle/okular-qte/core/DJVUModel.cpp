#include "DJVUModel.h"

using namespace okular;

DJVUModel::DJVUModel()
{
    m_djvu=new KDjVu();
}

DJVUModel::~DJVUModel()
{
    delete m_djvu;
}

int DJVUModel::open(QString filename)
{
    if(m_djvu->openFile(filename)) {
        QVector<KDjVu::Page*> pages=m_djvu->pages();
        totalPage=pages.size();
        pageRotate=0;
        currentPage=0;
        return 0;
    };
    return -1;
}

void DJVUModel::close()
{
    m_djvu->closeFile();
}

void DJVUModel::setPageByNo(int page)
{
    //currentPage ranges from 0, but viewer's page ranges from 1.
    currentPage=page-1;
    if (currentPage<0)
        currentPage=0;
    else if(currentPage>=totalPage)
        currentPage=totalPage-1;

    KDjVu::Page* curpage=m_djvu->pages().at(currentPage);
    DocumentSize.setHeight(curpage->height());
    DocumentSize.setWidth(curpage->width());

}

QImage DJVUModel::getCurrentImage(Qt::AspectRatioMode mode)
{
    QSize actsize=this->DocumentSize;
    actsize.scale(this->ViewSize,mode);

    return m_djvu->image(this->currentPage,actsize.width(),actsize.height(),this->pageRotate);
}

int DJVUModel::getCurrentPageNo()
{
    return this->currentPage+1;
}

QString DJVUModel::getCurrentPageName()
{
    return QString();
}

void DJVUModel::setPageByName(QString name)
{
    int page=m_djvu->pageNumber(name);
    //that function needs a hack, see above.
    this->setPageByNo(page+1);
}

int DJVUModel::getTotalPage()
{
   return totalPage;
}
