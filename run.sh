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
    id=$(docker run -d -v "/$path/Image:/root/Image" "$container" all EGCCFLAGS="-g -Wl,--oformat=elf64-x86-64" ELDFLAGS="--oformat=elf64-x86-64 -o /root/Image/kernel.elf")
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
    fi

    echo
    echo "Debug mode"
    echo "Quick! Run the following command in another terminal:"
    echo 'qemu-system-x86_64 -s -S -hda "/tmp/retos/x64BareBonesImage.qcow2" -m 512 -soundhw pcspk'

    read -p "Press enter to finish"
elif [ "$1" = "FAST_WSL" ]
then
    echo
    echo "Fast WSL mode"

    if [ "$(whoami)" = "tomyr" ]
    then
        echo "Copying files to //wsl$/Ubuntu/tmp/retos"
        rm -rf "//wsl$/Ubuntu/tmp/retos"
        mkdir "//wsl$/Ubuntu/tmp/retos"
        cp "$path/Image/"* "//wsl$/Ubuntu/tmp/retos"
    else
        echo "Copying files to /mnt/c/Users/Usuario/Documents/GitHub/RetOS/Image"
        cp "$path/Image/"* "/mnt/c/Users/Usuario/Documents/GitHub/RetOS/Image/"
    fi
    echo "Done"

    make clean -CToolchain > /dev/null
    make clean > /dev/null

    read -p "Press enter to finish"
else
    sudo qemu-system-x86_64 -hda "$path/Image/x64BareBonesImage.qcow2" -m 512 -soundhw pcspk
    make clean -CToolchain > /dev/null
    make clean > /dev/null
fi
