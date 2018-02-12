
source ../version

for folder in  */; do
    echo "--------------------------------------------------------------"
    echo "        Building: "$folder 
    echo " "
    
    cd $folder
#    make clean
    
    if [ "$_ARCH" == "Lin" ]; then
        qmake
    elif [ "$_ARCH" == "Win" ]; then
        /home/user/mxe/usr/i686-w64-mingw32.shared/qt5/bin/qmake
    fi
    
    make
    
    cd plugin
    cp -r ./* ../../../release/SimulIDE_$VERSION-$_ARCH$_BITS/plugins
    cd ../../
    echo "--------------------------------------------------------------"
done
