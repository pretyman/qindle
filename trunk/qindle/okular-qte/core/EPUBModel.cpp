#include "EPUBModel.h"
#include "epubreply.h"
#include <QXmlStreamReader>

using namespace okular;

EPUBModel::EPUBModel()
{
    m_zip=new QuaZip();
    currentPage=0;
}

EPUBModel::~EPUBModel()
{
    delete m_zip;
}

int EPUBModel::open(QString filename)
{
    m_zip->setZipName(filename);

    if(m_zip->open(QuaZip::mdUnzip,NULL)) {
        return init();
    } else
        qWarning("testRead(): zip.open(): %d", m_zip->getZipError());
    return -1;
}

void EPUBModel::close()
{
    m_zip->close();
}

QNetworkReply* EPUBModel::createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData)
{
    QNetworkReply* ret;
    qWarning("%s", req.url().toString().toAscii().data());
    if(req.url().scheme()=="file")
        ret=new EPUBReply(this,req,op,m_zip, opfpath);
    else
        ret=QNetworkAccessManager::createRequest(op,req,outgoingData);
    return ret;
}

QString EPUBModel::getCurrentPageName()
{
    //qwebpage's url must begin with '/'
    return "/"+Manifest.value(Spine.at(currentPage));
}

int EPUBModel::getCurrentPageNo()
{
    return currentPage;
}

void EPUBModel::setPageByName(QString name)
{
    QString id=Manifest.key(name);
    currentPage=Spine.indexOf(id);
}

void EPUBModel::setPageByNo(int page)
{
    currentPage=page;
}

int EPUBModel::init()
{
    //get opf file
    if(!(m_zip->setCurrentFile("META-INF/container.xml")))
        return -1;
    QuaZipFile ocffile(m_zip);
    ocffile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString opfname;
    QXmlStreamReader ocfreader(&ocffile);
    for(;!(ocfreader.atEnd());ocfreader.readNextStartElement()){
        if(ocfreader.name()=="rootfile") {
            opfname=ocfreader.attributes().value(QString(), "full-path").toString();
            break;
        }
    }
    ocfreader.clear();
    ocffile.close();
    if(opfname.isNull())
        return -1;

    if(opfname.contains("/"))
        opfpath=opfname.left(opfname.lastIndexOf("/")+1);
    else
        opfpath="";

    //parse opf file
    if(!(m_zip->setCurrentFile(opfname)))
        return -1;
    ocffile.open(QIODevice::ReadOnly | QIODevice::Text);
    //Some file have a "xml 1.1" heading, which streamreader doesn't like, so eat it.
    //qWarning("%s",ocffile.readLine(80).data());
    ocffile.readLine(255);
    ocfreader.setDevice(&ocffile);
    for(;!(ocfreader.atEnd());ocfreader.readNextStartElement()){
        //currently we do not handle metadata
        if(ocfreader.isEndElement())
            continue;
        //this is a ugly method. directly access elements.
        if(ocfreader.name()=="item")
            Manifest.insert(ocfreader.attributes().value(QString(), "id").toString(), ocfreader.attributes().value(QString(), "href").toString());
        else if(ocfreader.name()=="spine")
            tocID=ocfreader.attributes().value(QString(), "toc").toString();
        else if(ocfreader.name()=="itemref")
            Spine.append(ocfreader.attributes().value(QString(), "idref").toString());

    }
    ocfreader.clear();
    ocffile.close();

    return 0;
}
