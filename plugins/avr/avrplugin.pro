PLGN_DIR_DATA = $$PWD/resources/data
PLGN_DIR_EXAMPLES = $$PWD/resources/examples

include( ../commonplugin.pro )

TARGET  = $$qtLibraryTarget(avrplugin)


SOURCES =   $$PWD/src/*.cpp \
            avrplugin.cpp \
            src/read_elf.c
            
HEADERS =   $$PWD/src/*.h \
            avrplugin.h

INCLUDEPATH += src \
               $${TARGET_PREFIX}/include
               
RESOURCES = $$PWD/src/resources.qrc

#LIBS  += ./lib/simavr-$$_ARCH$$_BITS/libsimavr.a
                        
#DESTDIR = ../../release/SimulIDE_$$VERSION-$$_ARCH$$_BITS/plugins/AVRplugin/data/plugins

#QMAKE_CFLAGS += --std=gnu99

isEqual( _ARCH,"Lin") {

    #PKG_CONFIG = PKG_CONFIG_PATH=$$(QTDIR)/lib/pkgconfig pkg-config
    PKG_CONFIG = "PKG_CONFIG_PATH=$${TARGET_PREFIX}/lib/pkgconfig pkg-config"
    CONFIG += link_pkgconfig
    #CONFIG += c++11
    PKGCONFIG += simavr
    #QMAKE_LIBS += -lsimulide -lutil
    QMAKE_LIBS += -lutil
    #QMAKE_LFLAGS += -Wl,-export-dynamic

}
isEqual( _ARCH,"Win") {

    INCLUDEPATH += $${TARGET_PREFIX}/include/simavr
    #QMAKE_LIBS += -lsimulide -lws2_32 -lwsock32
    QMAKE_LIBS += -lsimavr -lelf -lws2_32 -lwsock32 -lglibc_win

    #LIBS += ../../release/$$TARGET_NAME/SimulIDE_0.1.40.dll
    #LIBS += /home/user/mxe-last/usr/i686-w64-mingw32.shared/bin/libwinpthread-1.dll
    #LIBS += /home/user/mxe/usr/i686-w64-mingw32.static/lib/libwsock32.a
    #LIBS += ../libsimulide.a
}

QMAKE_LIBS += -ldl
