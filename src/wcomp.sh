#!/bin/bash
CC="x86_64-w64-mingw32-gcc"
libs="-lmingw32 -lglfw3dll -lraylib -lm -lcurses"
$CC -Wall -DPDC_WIDE -DPDC_FORCE_UTF8 -DPDC_DLL_BUILD -o /tmp/Altair8800.exe io.c devices.c 8080.c panel.c mem.c main.c $libs && scp /tmp/Altair8800.exe $1@$2:
