#!/bin/sh
libs="-lrt -lraylib -lm -lcurses"
cc -Wall -I /usr/local/include -L /usr/local/lib -g -z muldefs $libs -o ../bin/Altair8800 devices.c io.c 8080.c panel.c mem.c main.c && ../bin/Altair8800
