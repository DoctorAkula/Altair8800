#!/bin/bash
gcc -g -z muldefs -lm -lraylib -o ../bin/Altair8800 8080.c panel.c mem.c main.c && ../bin/Altair8800
