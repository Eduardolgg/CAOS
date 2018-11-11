#!/bin/sh
cd ./build

if hash mesonconf 2>/dev/null; then
	mesonconf -Dbuildtype=release
	mesonconf -Db_coverage=false
else
	meson configure -Dbuildtype=release
	meson configure -Db_coverage=false
fi
