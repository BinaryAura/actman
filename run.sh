#!/usr/bin/env bash

CONFIG=Debug
PROJECT=actman

./premake5 --c=clang++ gmake
if [ $? != 0 ]; then
  exit
fi
make
if [ $? != 0 ]; then
  exit
fi
bin/$CONFIG/${PROJECT}/${PROJECT} $1 $2
