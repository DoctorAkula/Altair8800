#!/bin/bash
gcc -o ../bin/tests main.c ../../src/devices.c ../../src/io.c ../../src/mem.c ../../src/8080.c -I ../../src/ -lrt -lncurses && ../bin/tests
