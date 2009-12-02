#!/bin/sh

#qt的初始化脚本
/etc/init.d/framework stop
/etc/init.d/pmond stop
mount --bind /mnt/us/local /usr/local
export LD_LIBRARY_PATH=/usr/local/lib
export XDG_CONFIG_HOME=/mnt/us/local/Settings
export QWS_KEYBOARD=usb
/usr/local/bin/qwsserver -qws
