#!/bin/bash

CFLAGS="-g -Wall -Wextra -Werror -Wno-discarded-qualifiers -Wno-unused-parameter"
GTK_FLAGS=$(pkg-config --cflags --libs gtk4)

mkdir -p ../build
pushd ../build >> /dev/null
gcc ../source/gtk_rbyedit.c ../source/rbyedit.c  ../source/rbyinfo.c $CFLAGS $GTK_FLAGS -o debug
popd >> /dev/null
