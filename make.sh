#!/bin/bash

cd src
gcc -Wall -O3 -march=native -o CDOSVASP *.c
cd ..
mv src/CDOSVASP CDOSVASP
