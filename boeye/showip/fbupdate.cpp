#include "fbupdate.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>


FBUpdate::FBUpdate(QObject *parent) :
    QObject(parent)
{

}

FBUpdate::~FBUpdate()
{

}

int FBUpdate::Init()
{
    connect(&(this->timer), SIGNAL(timeout()), this, SLOT(timeOut()));
    this->timer.setInterval(FB_UPDATE_INTERVAL);
    this->timer.setSingleShot(1);
    this->fbhandle = open("/dev/fb0", O_RDWR);
    if(this->fbhandle) {
        return 1;
    } else {
        return 0;
    }
}

void FBUpdate::Update(int mode, QRect region)
{
    if(this->needupdate == 2) {
        return;
    }
    this->rect = this->rect.united(region);
    this->update_args.mode = mode;
    this->update_args.x = this->rect.x();
    this->update_args.y = this->rect.y();
    this->update_args.w = this->rect.width();
    this->update_args.h = this->rect.height();
    if(this->timer.isActive()) {
        this->needupdate = 1;
    } else {
        this->doUpdate();
        this->timer.start();
    }
}

void FBUpdate::Close()
{
    close(this->fbhandle);
}

void FBUpdate::Update(QRect region)
{
    this->Update(BS_UPD_MODE_MU, region);
}

void FBUpdate::StartAutoUpdate()
{
    this->update_args.mode = BS_UPD_MODE_MU;
    this->update_args.x = 0;
    this->update_args.y = 0;
    this->update_args.w = 0;
    this->update_args.h = 0;

    this->needupdate = 2;
    this->timer.start();
}

void FBUpdate::StopAutoUpdate()
{
    this->needupdate = 0;
    this->timer.stop();
}

void FBUpdate::doUpdate()
{
    int command = FBIO_UPD_DISPLAY_AREA;
    ioctl(this->fbhandle, command, &(this->update_args));
    this->rect = QRect();
    qDebug("Updated %d,%d,%d,%d", this->update_args.x, this->update_args.y, this->update_args.w, this->update_args.h);
}

void FBUpdate::timeOut()
{
    if(this->needupdate == 1) {
        this->needupdate = 0;
        this->doUpdate();
    }
}
