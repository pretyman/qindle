#!/bin/sh

#qt的初始化脚本
/etc/init.d/framework stop
/etc/init.d/pmond stop
mount --bind /var/local/local /usr/local
export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/Trolltech/QtEmbedded-4.5.2-arm/lib
export XDG_CONFIG_HOME=/mnt/us/local/Settings
export QWS_KEYBOARD=usb
/usr/local/bin/qwsserver -qws
