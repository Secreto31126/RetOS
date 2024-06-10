#!/bin/bash

# Only execute if the user is root and is in the folder /root
if [ "$(whoami)" = "root" ] && [ "$PWD" = "/root" ]
then
    find . -name "*.c" | while read line; do sed -i '1s/^\(.*\)$/\/\/ This is a personal academic project. Dear PVS-Studio, please check it.\n\1/' "$line"; done
	find . -name "*.c" | while read line; do sed -i '2s/^\(.*\)$/\/\/ PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https:\/\/pvs-studio.com\n\1/' "$line"; done
    exit 0
fi

echo "Invalid execution. DON'T RUN IT OUTSIDE OF DOCKER!!!"
exit 1
