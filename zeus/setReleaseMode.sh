#!/bin/sh
cd ./build
mesonconf -Dbuildtype=release
mesonconf -Db_coverage=false
