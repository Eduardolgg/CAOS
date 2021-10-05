#!/bin/sh

cd ./build
meson configure -h > grep "Error"
if [ "$?" -ne 0 ]; then
	mesonconf -Dbuildtype=debug
	mesonconf -Db_coverage=true
else
	meson configure -Dbuildtype=debug
	meson configure -Db_coverage=true
fi
