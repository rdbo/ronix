#!/bin/sh

set -e

mkdir -p build

echo "[*] Building imgui..."
if [ ! -d "build/imgui" ]; then
	mkdir -p build/imgui
	for f in $(find imgui -type f -name "*.cpp"); do
		obj="$(echo "$f" | sed 's|.*/||g' | sed 's/\.cpp$/.o/')"
		echo $obj
		c++ -o "build/imgui/$obj" -std=gnu++11 -m32 -fPIC -I /usr/include/SDL2 -I imgui -c "$f"
	done
fi

# echo "[*] Building PCH..."
# if [ ! -f "build/pch/pch.hpp.gch" ]; then
# 	mkdir -p "build/pch"
# 	c++ -o build/pch/pch.hpp.gch -std=gnu++11 -m32 -fpermissive -c -x c++-header ronix/pch.hpp -I /usr/include/SDL2 -I . -I imgui -I hl2sdk $(find hl2sdk -type d -exec printf "%s" "-I {} " \;) -I json
# fi

echo "[*] Building ronix..."
# if [ ! -e ronix/pch.hpp.gch ]; then
# 	ln ./build/pch/pch.hpp.gch ./ronix/pch.hpp.gch
# fi
c++ -o build/libronix.so \
	-std=gnu++11 -m32 -shared -fPIC -fpermissive -pthread \
	-I /usr/include/SDL2 -I . -I imgui -I ronix -I hl2sdk $(find hl2sdk -type d -exec printf "%s" "-I {} " \;) -I json \
	$(find ronix -type f -name "*.cpp") build/imgui/*.o \
	-Bstatic -lc -lgcc -lstdc++ -Bdynamic -lSDL2 -lGL
