PLGN_DIR_BIN = $$PWD/resources/bin
PLGN_DIR_DATA = $$PWD/resources/data
PLGN_DIR_EXAMPLES = $$PWD/resources/examples

include( ../commonplugin.pro )

TARGET  = $$qtLibraryTarget(picplugin)


SOURCES =   $$PWD/src/piccomponent.cpp \
            $$PWD/src/piccomponentpin.cpp \
            $$PWD/src/picprocessor.cpp \
            picplugin.cpp
            
HEADERS =   $$PWD/src/piccomponent.h \
            $$PWD/src/piccomponentpin.h \
            $$PWD/src/picprocessor.h \
            picplugin.h

INCLUDEPATH += src

isEqual( _ARCH,"Lin") {

    LIBS += -lgpsim

    CONFIG += link_pkgconfig
    PKGCONFIG += glib-2.0
}
isEqual( _ARCH,"Win") {
    
    INCLUDEPATH += /home/user/mxe/usr/i686-w64-mingw32.shared/lib/glib-2.0/include \
                   /home/user/mxe/usr/i686-w64-mingw32.shared/include/glib-2.0
    INCLUDEPATH += /home/user/mxe/usr/i686-w64-mingw32.shared/include
    
    LIBS +=  $$PWD/resources/bin/libgpsim.dll
}



