
#include( ../version )
_ARCH = Lin32

TEMPLATE = lib


INCLUDEPATH +=  ../../src \
                ../../src/gui \
                ../../src/gui/circuitwidget \
                ../../src/gui/circuitwidget/components \
                ../../src/gui/circuitwidget/components/mcu \
                ../../src/gui/oscopewidget \
                ../../src/gui/plotterwidget \
                ../../src/gui/terminalwidget \
                ../../src/gui/QPropertyEditor \
                ../../src/gui/editorwidget \
                ../../src/gui/editorwidget/findreplacedialog \
                ../../src/gui/serialwidget \
                ../../src/simulator \
                ../../src/simulator/elements \
                ../../src/simulator/elements/processors \
                

OBJECTS_DIR = build
MOC_DIR     = build

CONFIG  += plugin
CONFIG  += qt
CONFIG -= debug_and_release debug_and_release_target

QT += concurrent
QT += widgets
QT += xml
QT += gui

DEFINES += MYSHAREDLIB_LIBRARY

QMAKE_CFLAGS_SHLIB += -fpic

message( "-----------------------------" )
message( "       " $$_ARCH)
message( "-----------------------------" )
