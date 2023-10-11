#!/bin/bash
path="${PWD}"
container="tpe-builder"

docker build -t "$container" .
docker run -d -v "/$path/Image:/root/Image" --rm "$container"

danglings=$(docker images -f "dangling=true" -q)
if [ ! -z "$danglings" ]
then
    docker rmi "$danglings"
fi

qemu-system-x86_64 -hda "$path/Image/x64BareBonesImage.qcow2" -m 512
