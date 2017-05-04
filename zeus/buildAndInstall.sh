#!/bin/sh

./setReleaseMode.sh
cd ./build
ninja clean
ninja
ninja install
