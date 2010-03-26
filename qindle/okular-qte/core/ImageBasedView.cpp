#include "ImageBasedView.h"
#include <QPixmap>

using namespace okular;

ImageBasedView::ImageBasedView(QWidget *p) : QLabel(p)
{

}

int ImageBasedView::loadImage(QImage image)
{
    this->setPixmap(QPixmap::fromImage(image));
    return 0;
}
