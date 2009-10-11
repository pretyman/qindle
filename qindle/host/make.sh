#!/bin/bash

#从编译环境中复制目标文件，创建目录结构
mkdir local
cd local
mkdir bin
mkdir share
mkdir lib
mkdir -p Trolltech/QtEmbedded-4.5.2-arm/lib
cd Trolltech/QtEmbedded-4.5.2-arm
cp /usr/local/Trolltech/QtEmbedded-4.5.2-arm/lib/*.2 ./lib/
cp -R /usr/local/Trolltech/QtEmbedded-4.5.2-arm/lib/fonts ./lib/
cp -R /usr/local/Trolltech/QtEmbedded-4.5.2-arm/plugins ./
cp -R /usr/local/Trolltech/QtEmbedded-4.5.2-arm/translations ./
cd ../..
cp -R ../../Settings ./
cp ../../target/* ./bin/
cd ..
tar cvjf qindle.tar.bz2 ./local
