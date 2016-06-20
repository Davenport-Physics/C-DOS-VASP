#!/bin/bash

cd src
gcc -g -o CDOSVASP-debug *.c
cd ..
mv src/CDOSVASP-debug CDOSVASP-debug
