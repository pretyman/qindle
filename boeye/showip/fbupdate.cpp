#include "fbupdate.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>


FBUpdate::FBUpdate(QObject *parent) :
    QObject(parent)
{
    this->timer = new QTimer();
}

FBUpdate::~FBUpdate()
{
    delete this->timer;
}

int FBUpdate::Init()
{
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    this->timer->setInterval(FB_UPDATE_INTERVAL);
    this->timer->setSingleShot(0);
    this->fbhandle = open("/dev/fb0", O_RDWR);
    if(this->fbhandle) {
        return 1;
    } else {
        return 0;
    }

}

void FBUpdate::Update(int mode)
{
    this->Update(mode, 0, 0, 0, 0);
}

void FBUpdate::Update(int mode, int x, int y, int w, int h)
{
    if(this->needupdate == 2) {
        return;
    }
    this->update_args.mode = mode;
    this->update_args.x = x;
    this->update_args.y = y;
    this->update_args.w = w;
    this->update_args.h = h;
    if(this->timer->isActive()) {
        this->needupdate = 1;
    } else {
        this->timer->singleShot(FB_UPDATE_INTERVAL, this, SLOT(timeOut()));
        this->doUpdate();
    }
}

void FBUpdate::Close()
{
    close(this->fbhandle);
}

void FBUpdate::Update()
{
    this->Update(BS_UPD_MODE_GU, 0, 0, 0, 0);
}

void FBUpdate::StartAutoUpdate()
{
    this->update_args.mode = BS_UPD_MODE_GU;
    this->update_args.x = 0;
    this->update_args.y = 0;
    this->update_args.w = 0;
    this->update_args.h = 0;

    this->needupdate = 2;
    this->timer->start();
}

void FBUpdate::StopAutoUpdate()
{
    this->needupdate = 0;
    this->timer->stop();
}

void FBUpdate::doUpdate()
{
    int command = FBIO_UPD_DISPLAY_AREA;
    ioctl(this->fbhandle, command, &(this->update_args));
    //qDebug("Updated");
}

void FBUpdate::timeOut()
{
    if(this->needupdate == 1) {
        this->doUpdate();
        this->needupdate = 0;
    } else if(this->needupdate == 2) {
        this->doUpdate();
    }
}
