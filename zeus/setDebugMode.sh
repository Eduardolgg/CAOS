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
meson configure -h > grep "Error"
if [ "$?" -ne 0 ]; then
	mesonconf -Dbuildtype=debug
	mesonconf -Db_coverage=true
else
	meson configure -Dbuildtype=debug
	meson configure -Db_coverage=true
fi
