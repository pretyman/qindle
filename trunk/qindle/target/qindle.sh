#!/bin/sh

#qt的初始化脚本
/etc/init.d/framework stop
/etc/init.d/pmond stop
mount --bind /mnt/us/local /usr/local
export LD_LIBRARY_PATH=/usr/local/lib
export XDG_CONFIG_HOME=/mnt/us/local/Settings
export QWS_KEYBOARD="LinuxInput:/dev/input/event0 LinuxInput:/dev/input/event1"
/usr/local/bin/qwsserver -qws
