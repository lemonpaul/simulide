#! /bin/bash

source ../version

PLUGINS_DIR=`pwd`

for plugin_dir in  */; do

    echo ""
    echo ""
    echo "******************************************************"
    echo "        Building: "$plugin_dir" Plugin"
    echo "******************************************************"

    cd $PLUGINS_DIR/$plugin_dir
    ./build.sh

    echo ""
    echo "--------------------------------------------------------------"
    echo ""
done
