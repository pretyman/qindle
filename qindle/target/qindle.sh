#!/bin/sh

#qt的初始化脚本
/etc/init.d/framework stop
/etc/init.d/audio stop
mount | grep /usr/local > /dev/null
if [ $? -ne 0 ] ; then
	mount --bind /mnt/us/local /usr/local
fi
export LD_LIBRARY_PATH=/usr/local/lib
export XDG_CONFIG_HOME=/mnt/us/local/Settings
export QWS_KEYBOARD="LinuxInput:/dev/input/event0 LinuxInput:/dev/input/event1"
cd /mnt/us
while [ 1 ] 
do 
	/usr/local/bin/qwsserver -qws
	if [ $? -eq 0 ] ; then
	break
	fi
done
umount /usr/local
/etc/init.d/audio start
/etc/init.d/framework start

