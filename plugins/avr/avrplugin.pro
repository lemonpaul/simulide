PLGN_DIR_DATA = $$PWD/resources/data
PLGN_DIR_EXAMPLES = $$PWD/resources/examples

include( ../commonplugin.pro )

#system( cd $$PWD/src/simavr && make CROSS=$$_CROSS OBJ=$$_ARCH$$_BITS)
#system( cd ../../dependencies && ./build-simavr-$$_ARCH)

TARGET  = $$qtLibraryTarget(avrplugin)

SOURCES =   $$PWD/src/*.cpp \
            avrplugin.cpp \
            src/read_elf.c
            
HEADERS =   $$PWD/src/*.h \
            avrplugin.h

INCLUDEPATH += src \
               ../../dependencies/build-$$_ARCH$$_BITS/include/simavr
               
RESOURCES = $$PWD/src/resources.qrc

LIBS  += ../../../dependencies/build-$$_ARCH$$_BITS/lib/libsimavr.a

isEqual( _ARCH,"Lin") {
    QMAKE_LIBS += -lutil
}
isEqual( _ARCH,"Win") {

    INCLUDEPATH += ../../dependencies/build-$$_ARCH$$_BITS/include
    QMAKE_LFLAGS += "-L../../../dependencies/build-$$_ARCH$$_BITS/lib"
    QMAKE_LIBS += -lsimavr -lelf -lws2_32 -lwsock32 -lglibc_win
}

QMAKE_LIBS += -ldl
