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

echo "Generating config files"
cd ../genconfig/
sudo ./generate_config.sh

./setReleaseMode.sh $1
cd ./build
ninja clean
ninja
sudo ninja install
