#ifndef FBUPDATE_H
#define FBUPDATE_H

#include <QObject>
#include <QTimer>

#define BS_UPD_MODE_MU          1       //  MU (monochrome update)
#define BS_UPD_MODE_GU          2       //  GU (grayscale update)
#define BS_UPD_MODE_GC          3       //  GC (grayscale clear)
#define BS_UPD_MODE_PU          4       //  PU (pen update)

#define FBIO_UPD_DISPLAY_FULL   4621
#define FBIO_UPD_DISPLAY_AREA  4622

#define FB_UPDATE_INTERVAL 1000
#define FB_WIDTH 824
#define FB_HEIGHT 1200

class FBUpdate : public QObject
{
    Q_OBJECT
public:
    explicit FBUpdate(QObject *parent = 0);
    ~FBUpdate();
    int Init();
    void Update(int mode);
    void Update(int mode, int x, int y, int w, int h);
    void Close();

signals:

public slots:
    void Update();
    void StartAutoUpdate();
    void StopAutoUpdate();

private:
    int fbhandle;
    int needupdate;
    QTimer* timer;
    struct update_args_t {
        unsigned int    mode;
        unsigned int    x;
        unsigned int    y;
        unsigned int    w;
        unsigned int    h;
    } update_args;
    void doUpdate();

private slots:
    void timeOut();
};

#endif // FBUPDATE_H
