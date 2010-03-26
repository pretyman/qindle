#ifndef TextBasedModel_h
#define TextBasedModel_h

#include "AbstractModel.h"
#include <QNetworkAccessManager>
#include <QString>

namespace okular{
class TextBasedModel : public QNetworkAccessManager, public AbstractModel {
public:
    //Page 0 is the topic page.
protected:
    virtual QNetworkReply *	createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0 )=0;
    int currentPage;
    virtual int init()=0;
};
}
#endif // TextBasedModel_h
