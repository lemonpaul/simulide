
include( ../commonplugin.pro )

TARGET  = $$qtLibraryTarget( audioplugin )


SOURCES =   audioplugin.cpp \
            $$PWD/src/*.cpp
            
HEADERS =   audioplugin.h \
            $$PWD/src/*.h

INCLUDEPATH +=  src 
                
QT += multimedia widgets

RESOURCES = $$PWD/src/resources.qrc
