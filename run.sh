#!/bin/bash
path="${PWD}"
container="tpe-builder"

docker build -t "$container" .
docker run -d -v "/$path/Image:/root/Image" --rm "$container"
docker rmi $(docker images -f "dangling=true" -q)

qemu-system-x86_64 -hda "$path/Image/x64BareBonesImage.qcow2" -m 512
