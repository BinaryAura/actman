#!/usr/bin/env bash

CONFIG=Debug
PROJECT=actman

git submodule update --init

./premake5 gmake --cc=clang
if [ $? != 0 ]; then
  exit
fi
make
if [ $? != 0 ]; then
  exit
fi
bin/$CONFIG/${PROJECT}/${PROJECT} -v $1 $2 -w 16
