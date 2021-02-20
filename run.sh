#!/usr/bin/env bash

CONFIG=Debug
PROJECT=actman

git submodule update --init

./premake5 --cc=clang gmake
if [ $? != 0 ]; then
  exit
fi
make
if [ $? != 0 ]; then
  exit
fi
bin/$CONFIG/${PROJECT}/${PROJECT} -v $1 $2
