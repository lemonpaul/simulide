#! /bin/bash
#

echo " "
echo " "
echo "******************************************************"
echo "          Building Simulide Main Executable"
echo "******************************************************"
echo " "

i686-w64-mingw32.shared-qmake-qt5

make

echo ""
echo ""
echo "******************************************************"
echo "                  Building Plugins"
echo "******************************************************"
echo ""

./copydlls.sh

cd ../plugins

./build_plugins.sh


