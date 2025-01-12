#!/bin/sh

mkdir -p build

cc -o build/libronix.so -std=c++11 -m32 -shared -fPIC -I /usr/include/SDL2 -I . -I imgui -I hl2sdk $(find hl2sdk -type d -exec printf "%s" "-I {} " \;) -I ronix -I json -fpermissive $(find ronix -type f -name "*.cpp") $(find imgui -type f -name "*.cpp") -pthread
