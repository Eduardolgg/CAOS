#!/bin/sh
cd ./build
mesonconf -Dbuildtype=debug
mesonconf -Db_coverage=true
