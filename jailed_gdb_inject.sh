#!/bin/sh

GAME_NAME=hl2_linux
LIB_PATH=$(pwd)/build/libronix.so
LOAD_MODE=1 # RTLD_LAZY

if [ $EUID -ne 0 ]; then
	echo "[!] Please run as root"
	exit -1
fi

if [ ! -f $LIB_PATH ]; then
	echo "[!] Library does not exist"
	exit -1
fi

# Copy library to rootfs of jailed program
target_pid="$(pidof -s "$GAME_NAME")"
new_lib_path="/tmp/libronix.so"
cp "$LIB_PATH" "/proc/${target_pid}/root${new_lib_path}"

gdb -n -q -batch \
	-ex "attach $target_pid" \
	-ex "set \$dlopen = (void *(*)(char *, int))dlopen" \
	-ex "call \$dlopen(\"$new_lib_path\", $LOAD_MODE)" \
	-ex "detach" \
	-ex "quit"
