PLGN_DIR_DATA = $$PWD/resources/data
#PLGN_DIR_EXAMPLES = $$PWD/resources/examples

include( ../commonplugin.pro )

TARGET  = $$qtLibraryTarget(editorplugin)


SOURCES =   editorplugin.cpp \
            $$PWD/src/*.cpp \
            $$PWD/src/findreplacedialog/*.cpp
            
HEADERS =   editorplugin.h \
            $$PWD/src/*.h \
            $$PWD/src/findreplacedialog/*.h

INCLUDEPATH +=  ./ \
                src \
                src/findreplacedialog
                
#FORMS += src/findreplacedialog/findreplacedialog.ui 
#FORMS += src/findreplacedialog/findreplaceform.ui 

#DESTDIR = ../../release/SimulIDE_$$VERSION-$$_ARCH$$_BITS/plugins/EDITORplugin/data/plugins

