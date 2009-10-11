#!/bin/sh

#安装脚本，用于创建符号链接
cd /var/local
mkdir local
cd local
ln -s /mnt/us/local/bin ./
ln -s /mnt/us/local/share ./
ln -s /mnt/us/local/lib ./
mkdir -p Trolltech/QtEmbedded-4.5.2-arm
cd Trolltech/QtEmbedded-4.5.2-arm
ln -s /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/plugins ./
ln -s /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/translations ./
mkdir lib
cd lib
ln -s /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/lib/libQtCore.so.4.5.2 ./
ln -s libQtCore.so.4.5.2 libQtCore.so.4.5
ln -s libQtCore.so.4.5.2 libQtCore.so.4
ln -s libQtCore.so.4.5.2 libQtCore.so

ln -s /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/lib/libQtGui.so.4.5.2 ./
ln -s libQtGui.so.4.5.2 libQtGui.so.4.5
ln -s libQtGui.so.4.5.2 libQtGui.so.4
ln -s libQtGui.so.4.5.2 libQtGui.so

ln -s /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/lib/libQtNetwork.so.4.5.2 ./
ln -s libQtNetwork.so.4.5.2 libQtNetwork.so.4.5
ln -s libQtNetwork.so.4.5.2 libQtNetwork.so.4
ln -s libQtNetwork.so.4.5.2 libQtNetwork.so

ln -s /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/lib/libQtScript.so.4.5.2 ./
ln -s libQtScript.so.4.5.2 libQtScript.so.4.5
ln -s libQtScript.so.4.5.2 libQtScript.so.4
ln -s libQtScript.so.4.5.2 libQtScript.so

ln -s /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/lib/libQtSql.so.4.5.2 ./
ln -s libQtSql.so.4.5.2 libQtSql.so.4.5
ln -s libQtSql.so.4.5.2 libQtSql.so.4
ln -s libQtSql.so.4.5.2 libQtSql.so

ln -s /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/lib/libQtTest.so.4.5.2 ./
ln -s libQtTest.so.4.5.2 libQtTest.so.4.5
ln -s libQtTest.so.4.5.2 libQtTest.so.4
ln -s libQtTest.so.4.5.2 libQtTest.so

ln -s /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/lib/libQtXml.so.4.5.2 ./
ln -s libQtXml.so.4.5.2 libQtXml.so.4.5
ln -s libQtXml.so.4.5.2 libQtXml.so.4
ln -s libQtXml.so.4.5.2 libQtXml.so

ln -s /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/lib/fonts ./
#以下为未来添加字体的命令
#mkdir fonts
#cd fonts
#for i in `ls -1 /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/lib/fonts/`
#do
#ln -s /mnt/us/local/Trolltech/QtEmbedded-4.5.2-arm/lib/fonts/$i ./
#done