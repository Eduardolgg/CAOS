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

if [ "$debug" != "true" ]; then
  sed -i "/conf.set('DEBUG', true)/c conf.set('DEBUG', false)" include/meson.build
else
  sed -i "/conf.set('DEBUG', false)/c conf.set('DEBUG', true)" include/meson.build
fi

cd ./build
if hash mesonconf 2>/dev/null; then
	mesonconf -Dbuildtype=release
	mesonconf -Db_coverage=false
	mesonconf -DDEBUG=$debug
else
	meson configure -Dbuildtype=release
	meson configure -Db_coverage=false
	meson configure -DDEBUG=$debug
fi
