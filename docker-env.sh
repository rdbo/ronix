#!/bin/sh

set -e

docker build --platform linux/i386 -t ronix .
docker run --platform linux/i386 -v "$(pwd):/app" -it ronix
