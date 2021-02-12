#!/usr/bin/env bash

CONFIG=Debug
PROJECT=actman

./premake5 gmake
make
bin/$CONFIG/${PROJECT}/${PROJECT} $1 $2
