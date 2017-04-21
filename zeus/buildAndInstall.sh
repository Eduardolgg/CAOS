#!/bin/sh

./setReleaseMode.sh
cd ./build
ninja
ninja install
