#!/bin/bash

cd src
gcc -Wall -mtune=generic -m64 -o CDOSVASP *.c
cd ..
mv src/CDOSVASP CDOSVASP-Generic
