#!/bin/sh
cd ./build

if hash mesonconf 2> /dev/null; then
	mesonconf -Dbuildtype=debug
	mesonconf -Db_coverage=true
else
	meson configure -Dbuildtype=debug
	meson configure -Db_coverage=true
fi
