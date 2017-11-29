#! /bin/bash

source ../../version


$_CROSS$QMAKE_EXEC
cd build_$_ARCH$_BITS
make
