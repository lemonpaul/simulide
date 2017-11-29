#PLGN_DIR_DATA = $$PWD/resources/data
#PLGN_DIR_EXAMPLES = $$PWD/resources/examples

include( ../commonplugin.pro )

TARGET  = $$qtLibraryTarget(serialplugin)


SOURCES =   serialplugin.cpp \
            $$PWD/src/*.cpp
            
HEADERS =   serialplugin.h \
            $$PWD/src/*.h

INCLUDEPATH +=  src 
                
FORMS += $$PWD/src/serialportwidget.ui

QT += serialport

#DESTDIR = ../../release/SimulIDE_$$VERSION-$$_ARCH$$_BITS/plugins/SERIALplugin/data/plugins

