#!/bin/sh

git pull
mkdir -p build/
cd build/
if [ ! -f Makefile ]
then
  cmake -DCMAKE_BUILD_TYPE=Release ..
fi

make -j4 tgc-vp
