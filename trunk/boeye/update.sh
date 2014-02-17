#!/bin/sh
# This script was generated using Makeself 2.1.5
eecho ()
{
        echo "$1"
        echo "$1" > ${FIFO}
}

MACH=$(echo $(uname -m) | cut -c 1-3)
test "$MACH" != "arm" && echo "not support machine." && exit 1

        eecho "Start update ..."
	syspart=$(awk -F: '/system/ {print $1}' /proc/mtd)
	mount /dev/${syspart/mtd/mtdblock} /system
	sed -i 's:#\t/media:\t/media:' /system/etc/init.d/dbus-wait.sh
	cp /system/etc/init.d/dbus-wait.sh ./
	umount /system
	sync
	eecho "Finished"

