#!/bin/bash
cd build

if hash mesontest 2>/dev/null; then
	mesontest --list
else
	meson test --list
fi

echo ""
echo "**************************"
echo ""
echo "run"
echo "$> cd build"
echo "$> mesontest --gdb \"test name\""
echo "To get zeus runing in gdb"
