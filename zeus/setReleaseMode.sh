#!/bin/sh

cd ./build
meson configure -h > grep "Error"
if [ "$?" -ne 0 ]; then
	mesonconf -Dbuildtype=release
	mesonconf -Db_coverage=false
else
	meson configure -Dbuildtype=release
	meson configure -Db_coverage=false
fi
