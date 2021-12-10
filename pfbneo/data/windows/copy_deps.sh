#!/bin/sh

if [ -z "$1" ] || [ -z "$2" ]
	then echo "missing argument"
	exit 1
fi

for lib in `ldd $1 | grep mingw64 | awk '{print $3}'`
do
	cp -f "$lib" "$2" > /dev/null 2>&1
done

exit 0
