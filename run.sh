#!/bin/bash
path="${PWD}"
container="tpe-builder"

make clean -CToolchain
make clean
clear

docker build -t "$container" .
if [ $1 -ne 0 ]
then
    exit 1
fi

id=$(docker run -d -v "/$path/Image:/root/Image" "$container")
docker wait "$id"

clear
docker logs "$id"
docker rm "$id"

qemu-system-x86_64 -hda "$path/Image/x64BareBonesImage.qcow2" -m 512
