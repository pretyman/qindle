#include "CHMModel.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include "chmreply.h"
#include <QByteArray>
#include <string>

#define DATA_SIZE 512

using namespace okular;

int CHMModel::open(QString filename)
{
    m_file=chm_open(filename.toAscii().data());
    if(m_file==NULL)
        return -1;
    currentPage=0;
    return init();
}

void CHMModel::close()
{
    chm_close(m_file);
}

QNetworkReply* CHMModel::createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData)
{
    QNetworkReply* ret;
    //qWarning("%s", req.url().toString().toAscii().data());
    if(req.url().scheme()=="file")
        ret=new CHMReply(this,req, op, m_file);
    else
        ret=QNetworkAccessManager::createRequest(op,req,outgoingData);
    return ret;
}

int CHMModel::init()
{
    chmUnitInfo cui;
    int i=chm_resolve_object(m_file, "/#SYSTEM",&cui);
    if(i==CHM_RESOLVE_FAILURE)
        return -1;
    char data[DATA_SIZE]="";
    QByteArray filename;
    chm_retrieve_object(m_file, &cui, (unsigned char *)data, 0, DATA_SIZE);
    //the following decodes topic file name from #system file.
    char* code;
    char* length;
    while(i<DATA_SIZE) {
        code=data+i;
        length=data+i+2;
        if (*code==0x02) {
            filename.append(data+i+4,(int)*length);
            break;
        } else {
            i=i+4+(int)*length;
        } ;
    }
    TopicName=QString(filename);
    filename.clear();

    i=chm_resolve_object(m_file, "/#STRINGS",&cui);
    if(i==CHM_RESOLVE_FAILURE)
        return -1;
    chm_retrieve_object(m_file, &cui, (unsigned char *)data, 0, DATA_SIZE);
    //the following decodes toc file name from #strings file.
    length=data;
    while(i<3) {
        code=length;
        length=strchr(code+1,0);
        i++;
    };
    filename.append(code+1,length-code);
    TOCName=QString(filename);
    return 0;
}
QString CHMModel::getCurrentPageName()
{
    return "/"+TopicName;
}

int CHMModel::getCurrentPageNo()
{
    return 0;
}

void CHMModel::setPageByNo(int page)
{
    //CHM don't use page number.
}

void CHMModel::setPageByName(QString name)
{
    TopicName=name;
}
