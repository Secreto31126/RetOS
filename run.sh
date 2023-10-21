#!/bin/bash
path="${PWD}"
container="tpe-builder"

docker build -t "$container" .
if [ $? -ne 0 ]
then
    exit 1
fi

id=
if [ "$1" = "DEBUG" ]
then
    echo "Debug mode"
    id=$(docker run -d -v "/$path/Image:/root/Image" "$container" all EGCCFLAGS="-g -Wl,--oformat=elf64-x86-64" ELDFLAGS="--oformat=elf64-x86-64 -o kernel.elf")
else
    id=$(docker run -d -v "/$path/Image:/root/Image" "$container" all)
fi
docker wait "$id"

clear
docker logs "$id"
docker rm "$id" > /dev/null

if [ "$1" = "DEBUG" ]
then
    echo "Debug mode"
    echo "Quick! Run the following command in another terminal:"
    echo 'qemu-system-x86_64 -s -S -hda "./Image/x64BareBonesImage.qcow2" -m 512'
else
    qemu-system-x86_64 -hda "$path/Image/x64BareBonesImage.qcow2" -m 512 -d int
    make clean -CToolchain > /dev/null
    make clean > /dev/null
fi
