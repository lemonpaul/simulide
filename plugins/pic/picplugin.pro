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
                        

#DESTDIR = ../../release/SimulIDE_$$VERSION-$$_ARCH$$_BITS/plugins/PICplugin/data/plugins

isEqual( _ARCH,"Lin") {

    LIBS += -lgpsim
    
#    INCLUDEPATH +=  /usr/local/include/gpsim \
#        /usr/include/gpsim

    CONFIG += link_pkgconfig
    PKGCONFIG += glib-2.0

    
#    isEqual( _BITS,"32") {
#
#        INCLUDEPATH +=  /usr/lib/i386-linux-gnu/glib-2.0/include
#    }
#    isEqual( _BITS,"64") {
#
#        INCLUDEPATH +=  /usr/lib/x86_64-linux-gnu/glib-2.0/include
#    }

}
isEqual( _ARCH,"Win") {
    
    INCLUDEPATH +=  /home/user/mxe/usr/i686-w64-mingw32.static/include/glib-2.0 \
                    /home/user/mxe/usr/i686-w64-mingw32.static/lib/glib-2.0/include \
                    /home/user/mxe/usr/include
                    
    LIBS += /home/user/mxe/usr/i686-w64-mingw32.static/lib/libsrc.a 
    
    LIBS += /home/user/mxe-last/usr/i686-w64-mingw32.shared/bin/libglib-2.0-0.dll
}



