#!/bin/bash

apt-get update
apt-get install -y libc6-armel-cross libc6-dev-armel-cross binutils-arm-linux-gnueabi libncurses5-dev gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf
apt-get install -y g++
apt-get install -y build-essential
apt-get install -y make
apt-get install -y git
apt-get install -y cmake
apt-get install -y unzip

pushd /tmp
git clone https://github.com/KjellKod/g3log.git -b master
pushd g3log
pushd 3rdParty/gtest
unzip gtest-1.7.0.zip
popd
mkdir build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
make install
popd
popd
popd