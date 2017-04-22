#!/bin/sh

./setDebugMode.sh
rm -rf ./tests/fake_init.d/log/*

cd ./build
ninja
ninja test
ninja coverage-xml
ninja coverage-text
rm -rf ./meson-logs/coveragereport/
ninja coverage-html

mesontest --wrap=valgrind
