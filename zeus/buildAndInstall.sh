#!/bin/sh

./setReleaseMode.sh
cd ./build
ninja clean
ninja
sudo ninja install
