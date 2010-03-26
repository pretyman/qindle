#ifndef ImageBasedView_h
#define ImageBasedView_h

#include <QLabel>

namespace okular{
class ImageBasedView : public QLabel {
public:
    ImageBasedView(QWidget *p);
    int loadImage(QImage image);
//    int loadText(QString text);
};
}
#endif // ImageBasedView_h
