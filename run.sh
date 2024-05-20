#!/bin/bash
path="${PWD}"
container="tpe-builder"

if [ "$1" = "DOCS" ]
then
    doxygen Doxyfile
    exit 1
fi

docker build -t "$container" .
if [ $? -ne 0 ]
then
    exit 1
fi

id=
if [ "$1" = "DEBUG" ]
then
    echo "Debug mode"
    id=$(docker run -d -v "/$path/Image:/root/Image" "$container" all DEBUG=1)
else
    id=$(docker run -d -v "/$path/Image:/root/Image" "$container" all)
fi
docker wait "$id"

# clear
docker logs "$id"
docker rm "$id" > /dev/null

if [ "$1" = "DEBUG" ]
then
    if [ "$(whoami)" = "tomyr" ]
    then
        echo "Copying files to //wsl$/Ubuntu/tmp/retos"
        rm -rf "//wsl$/Ubuntu/tmp/retos"
        mkdir "//wsl$/Ubuntu/tmp/retos"
        echo $path
        cp "$path/Image/"* "//wsl$/Ubuntu/tmp/retos"
    else
        echo "Copying files to /tmp/retos"
        rm -rf "/tmp/retos"
        mkdir "/tmp/retos"
        cp "$path/Image/"* "/tmp/retos"
    fi

    echo
    echo "Debug mode"
    echo "Run the following command in another terminal:"
    echo 'qemu-system-x86_64 -s -S -hda "/tmp/retos/x64BareBonesImage.qcow2" -m 512 -soundhw pcspk'

    read -p "Press enter to finish"
elif [ "$1" = "FAST_WSL" ]
then
    echo
    echo "Fast WSL mode"

    echo "Copying files to //wsl$/Ubuntu/tmp/retos"
    rm -rf "//wsl$/Ubuntu/tmp/retos"
    mkdir "//wsl$/Ubuntu/tmp/retos"
    cp "$path/Image/"* "//wsl$/Ubuntu/tmp/retos"

    echo "Done"

    make clean -CToolchain > /dev/null
    make clean > /dev/null

    read -p "Press enter to finish"
else
    echo "Copying files to /tmp/retos"
    rm -rf "/tmp/retos"
    mkdir "/tmp/retos"
    cp "$path/Image/"* "/tmp/retos"

    qemu-system-x86_64 -hda "/tmp/retos/x64BareBonesImage.qcow2" -m 512 -soundhw pcspk -serial stdio
    make clean -CToolchain > /dev/null
    make clean > /dev/null
fi
