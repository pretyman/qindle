#ifndef TextBasedView_h
#define TextBasedView_h

#include <QWebView>
#include <QNetworkAccessManager>

namespace okular {
    class TextBasedView : public QWebView {
    public:
        TextBasedView(QWidget *p);

        int loadFile(QString filename);
        void setManager(QNetworkAccessManager* manager);
    };
}
#endif // TextBasedView_h
