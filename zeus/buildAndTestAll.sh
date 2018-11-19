#!/bin/sh

if [ "$#" -ne 0 ]; then
  if [ "$1" != "debug" ]; then
    echo "Usage $0 [debug]"
    exit 1
  else
    debug=true
  fi
fi

debug=${debug-false}
./setDebugMode.sh $1
rm -rf ./tests/fake_init.d/log/*

cd ./build
ninja clean
ninja
ninja test
ninja coverage-xml
ninja coverage-text
rm -rf ./meson-logs/coveragereport/
ninja coverage-html

if hash mesontest 2>/dev/null; then
	#mesontest --wrap=valgrind
	mesontest --setup=valgrind_trace_children
else
	meson test --setup=valgrind_trace_children
fi
