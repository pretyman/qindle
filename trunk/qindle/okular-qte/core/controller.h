#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString>
#include <QSharedPointer>
#include <QWidget>

namespace okular {
    class TextBasedView;
    class ImageBasedView;
    class AbstractModel;

    class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();
    ~Controller();
    QWidget* getView(QWidget *p);
    enum ViewerType {ImageBased, TextBased, OCRText};
    int getCurrentPageNo();
public slots:
    //For QFileDialog
    void openFile(QString filename);

    void gotoPage(int page);

private:
    ImageBasedView* imageview;
    TextBasedView* textview;
    ViewerType m_type;
    QSharedPointer<AbstractModel>* m_model;
signals:
    void finished(int ret);

};
}

#endif // CONTROLLER_H
