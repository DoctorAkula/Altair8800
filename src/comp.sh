#!/bin/bash
gcc -g -z muldefs -lrt -lm -lraylib -o ../bin/Altair8800 devices.c io.c 8080.c panel.c mem.c main.c && ../bin/Altair8800
