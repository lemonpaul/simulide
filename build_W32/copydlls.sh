#!/bin/bash
#
source ../version

TRGT_KEY="i686-w64-mingw32.shared"
TOOLCHAIN_DIR=$(dirname "`which ${TRGT_KEY}-gcc`")
TOOLCHAIN_DIR=$(dirname "$TOOLCHAIN_DIR")
echo "MXEDIR=$TOOLCHAIN_DIR"


QTDIR=$TOOLCHAIN_DIR/$TRGT_KEY/qt5

echo " "
echo " "
echo "******************************************************"
echo "          Copy needed .dll to bin folder"
echo "******************************************************"
echo " "

cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libbz2.dll          "$TARGET_PREFIX"/bin
#cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libdl.dll "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libfreetype-6.dll   "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libgcc_s_seh-1.dll  "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libgcc_s_sjlj-1.dll "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libglib-2.0-0.dll   "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libharfbuzz-0.dll   "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libiconv-2.dll      "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libintl-8.dll       "$TARGET_PREFIX"/bin
#cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libjpeg-9.dll "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libpcre-1.dll       "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libpcre2-16-0.dll   "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libpcre16-0.dll     "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libpng16-16.dll     "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libstdc++-6.dll     "$TARGET_PREFIX"/bin
#cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libwinpthread-1.dll "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/zlib1.dll           "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/libeay32.dll        "$TARGET_PREFIX"/bin
cp "$TOOLCHAIN_DIR"/$TRGT_KEY/bin/ssleay32.dll        "$TARGET_PREFIX"/bin

cp "$QTDIR"/bin/Qt5Core.dll                           "$TARGET_PREFIX"/bin
cp "$QTDIR"/bin/Qt5Gui.dll                            "$TARGET_PREFIX"/bin
#cp "$QTDIR"/bin/Qt5Svg.dll "$TARGET_PREFIX"/bin
cp "$QTDIR"/bin/Qt5Widgets.dll                        "$TARGET_PREFIX"/bin
cp "$QTDIR"/bin/Qt5Xml.dll                            "$TARGET_PREFIX"/bin
cp "$QTDIR"/bin/Qt5SerialPort.dll                     "$TARGET_PREFIX"/bin
cp "$QTDIR"/bin/Qt5Network.dll                        "$TARGET_PREFIX"/bin
cp "$QTDIR"/bin/Qt5Multimedia.dll                     "$TARGET_PREFIX"/bin

mkdir "$TARGET_PREFIX"/bin/plugins
cp -r $QTDIR/plugins/platforms                        "$TARGET_PREFIX"/bin/plugins/platforms
cp -r $QTDIR/plugins/mediaservice                      "$TARGET_PREFIX"/bin/plugins/mediaservice
cp -r $QTDIR/plugins/audio                            "$TARGET_PREFIX"/bin/plugins/audio
