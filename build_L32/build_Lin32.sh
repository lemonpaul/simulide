#! /bin/bash

echo " "
echo " "
echo "******************************************************"
echo "          Building Simulide Main Executable"
echo "******************************************************"
echo " "

qmake

make

echo ""
echo ""
echo "******************************************************"
echo "                  Building Plugins"
echo "******************************************************"
echo ""


cd ../plugins

./build_plugins.sh




