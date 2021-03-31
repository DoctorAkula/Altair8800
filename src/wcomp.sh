#!/bin/bash
CC="x86_64-w64-mingw32-gcc"
libs="-lmingw32 -lglfw3dll -lraylib -lm"
$CC  -o /tmp/Altair8800.exe 8080.c panel.c mem.c main.c $libs && scp /tmp/Altair8800.exe $1@$2:
