#!/bin/sh

#恢复amazon framework
umount /usr/local
/etc/init.d/pmond start
/etc/init.d/framework start