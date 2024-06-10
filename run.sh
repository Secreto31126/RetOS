#!/bin/bash
path="${PWD}"
container="tpe-builder"

if [ $# -eq 0 ] || [ "$1" = "-h" ]
then
    echo "Usage: $0 (build|debug|pvs|docs) [naive|buddy] [wsl]"
    echo
    echo "build: Build the image"
    echo "debug: Run the image in debug mode"
    echo "pvs: Run the image in PVS mode"
    echo "docs: Generate the documentation"
    echo "naive: Run the image with naive memory system"
    echo "buddy: Run the image with buddy memory system"
    exit 1
fi

if [ "$1" = "docs" ]
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
if [[ "$1" = b* ]]
then
    if [[ "$2" = b* ]]
    then
        echo "BUILD mode with buddy"
        id=$(docker run -d -v "/$path/Image:/root/Image" "$container" all BUDDY=1)
    else
        echo "BUILD mode"
        id=$(docker run -d -v "/$path/Image:/root/Image" "$container" all)
    fi
elif [[ "$1" = d* ]]
then
    if [[ "$2" = b* ]]
    then
        echo "DEBUG mode with buddy"
        id=$(docker run -d -v "/$path/Image:/root/Image" "$container" all DEBUG=1 BUDDY=1)
    else
        echo "DEBUG mode"
        id=$(docker run -d -v "/$path/Image:/root/Image" "$container" all DEBUG=1)
    fi
elif [[ "$1" = p* ]]
then
    if [[ "$2" = b* ]]
    then
        echo "PVS mode with buddy"
        id=$(docker run -d -v "/$path/Image:/root/Image" "$container" pvs BUDDY=1)
    else
        echo "PVS mode"
        id=$(docker run -d -v "/$path/Image:/root/Image" "$container" pvs)
    fi
else
    echo "Invalid mode"
    exit 1
fi

docker wait "$id"

# clear
docker logs "$id"
docker rm "$id" > /dev/null
docker image rm "$container" > /dev/null

if [[ "$1" = b* ]]
then
    if [[ "$3" = w* ]]
    then
        echo "Copying files to //wsl$/Ubuntu/tmp/retos"
        rm -rf "//wsl$/Ubuntu/tmp/retos"
        mkdir "//wsl$/Ubuntu/tmp/retos"
        cp "$path/Image/"* "//wsl$/Ubuntu/tmp/retos"

        echo "Done"

        make clean -CToolchain > /dev/null
        make clean > /dev/null
    else
        echo "Copying files to /tmp/retos"
        rm -rf "/tmp/retos"
        mkdir "/tmp/retos"
        cp "$path/Image/"* "/tmp/retos"

        qemu-system-x86_64 -hda "/tmp/retos/x64BareBonesImage.qcow2" -m 512 -soundhw pcspk -serial stdio
        make clean -CToolchain > /dev/null
        make clean > /dev/null
    fi
elif [[ "$1" = d* ]]
then
    if [[ "$3" = w* ]]
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

    echo "Run the following command in another terminal:"
    echo 'qemu-system-x86_64 -s -S -hda "/tmp/retos/x64BareBonesImage.qcow2" -m 512 -soundhw pcspk'
fi

exit 0
