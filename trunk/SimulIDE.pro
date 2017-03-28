
TEMPLATE = app

SOURCES += ../src/*.cpp \
    ../src/gui/*.cpp \
    ../src/gui/circuitwidget/*.cpp \
    ../src/gui/circuitwidget/components/*.cpp \
    ../src/gui/circuitwidget/components/mcu/*.cpp \
    ../src/gui/oscopewidget/*.cpp \
    ../src/gui/plotterwidget/*.cpp \
    ../src/gui/terminalwidget/*.cpp \
    ../src/gui/QPropertyEditor/*.cpp \
    ../src/gui/editorwidget/*.cpp \
#    ../src/gui/editorwidget/findreplacedialog/*.cpp \
    ../src/gui/serialwidget/*.cpp \
    ../src/simulator/*.cpp \
    ../src/simulator/elements/*.cpp \
    ../src/simulator/elements/processors/*.cpp \

HEADERS += ../src/*.h \
    ../src/gui/*.h \
    ../src/gui/circuitwidget/*.h \
    ../src/gui/circuitwidget/components/*.h \
    ../src/gui/circuitwidget/components/mcu/*.h \
    ../src/gui/oscopewidget/*.h \
    ../src/gui/plotterwidget/*.h \
    ../src/gui/terminalwidget/*.h \
    ../src/gui/QPropertyEditor/*.h \
    ../src/gui/editorwidget/*.h \
#    ../src/gui/editorwidget/findreplacedialog/*.h \
    ../src/gui/serialwidget/*.h \
    ../src/simulator/*.h \
    ../src/simulator/elements/*.h \
    ../src/simulator/elements/processors/*.h \

INCLUDEPATH += ../src \
    ../src/gui \
    ../src/gui/circuitwidget \
    ../src/gui/circuitwidget/components \
    ../src/gui/circuitwidget/components/mcu \
    ../src/gui/oscopewidget \
    ../src/gui/plotterwidget \
    ../src/gui/terminalwidget \
    ../src/gui/QPropertyEditor \
    ../src/gui/editorwidget \
#    ../src/gui/editorwidget/findreplacedialog \
    ../src/gui/serialwidget \
    ../src/simulator \
    ../src/simulator/elements \
    ../src/simulator/elements/processors \
    ../include/simavr/sim \
    ../include/gpsim
    #/usr/local/include/gpsim\
    #/usr/include/glib-2.0\
    #/usr/lib/i386-linux-gnu/glib-2.0/include\
    #/usr/lib/x86_64-linux-gnu/glib-2.0/include \
    
#FORMS += ../src/gui/editorwidget/findreplacedialog/*.ui 
FORMS += ../src/gui/serialwidget/serialportwidget.ui

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CXXFLAGS += -Wno-unused-parameter

RESOURCES = ../src/application.qrc
# TRANSLATIONS += SimulIDE.ts

QT += concurrent
QT += widgets
QT += xml
QT += gui
QT += serialport

CONFIG += qt \
    warn_on \
#    debug
CONFIG += static

CONFIG -= debug_and_release debug_and_release_target

message( $$_ARCH $$_PICC)

BUILD_DIR    = build
INCLUDEPATH += $$BUILD_DIR 
OBJECTS_DIR  = $$BUILD_DIR 
MOC_DIR      = $$BUILD_DIR 

isEqual( _PICC,"no") { 
    DEFINES += NO_PIC 
}
isEqual( _ARCH,"Lin32") {
    LIBS += ../include/simavr/obj-i686-linux-gnu/libsimavr.a
    LIBS += -lutil
    LIBS += -lpthread
    
    INCLUDEPATH += ../include/simavr/sim
    
    QMAKE_LFLAGS += -Wl,-export-dynamic
    
    isEqual( _PICC,"yes") {
        TARGET = ../release/SimulIDE_0.1.4-Lin32/SimulIDE_0.1.4
        INCLUDEPATH +=  /usr/include/glib-2.0\
                        /usr/lib/i386-linux-gnu/glib-2.0/include
                        
        LIBS += /usr/lib/libgpsim.so 
    } else { 
        TARGET = ../release/SimulIDE_0.1.4-Lin32-NO_PIC/SimulIDE_0.1.4
    }
}
isEqual( _ARCH,"Lin64") {
    LIBS += ../include/simavr/obj-x86_64-linux-gnu/libsimavr.a
    LIBS += -lutil
    LIBS += -lpthread
    
    INCLUDEPATH += ../include/simavr/sim 
    
    QMAKE_LFLAGS += -Wl,-export-dynamic
    
    isEqual( _PICC,"yes"){ 
        TARGET = ../release/SimulIDE_0.1.4-Lin64/SimulIDE_0.1.4
        INCLUDEPATH +=  /usr/include/glib-2.0\
                        /usr/lib/x86_64-linux-gnu/glib-2.0/include
                        
        LIBS += /usr/lib/libgpsim.so 
    }else { 
        TARGET = ../release/SimulIDE_0.1.4-Lin64-NO_PIC/SimulIDE_0.1.4
    }
}
isEqual( _ARCH,"Win32") {
    DEFINES += Win 
    Release:DESTDIR = ../release
    
    QMAKE_LIBS += -lws2_32
    QMAKE_LIBS += -lpthread
    
    LIBS += ../include/simavr/obj-i686-w64-mingw32.static/libsimavr.a
    LIBS += /home/user/mxe/usr/i686-w64-mingw32.static/lib/libpthread.a
    
    INCLUDEPATH += ../include/simavr/sim
    
    isEqual( _PICC,"yes"){
        
        INCLUDEPATH +=  /home/user/mxe/usr/i686-w64-mingw32.static/include/glib-2.0 \
                        /home/user/mxe/usr/i686-w64-mingw32.static/lib/glib-2.0/include \
                        /home/user/mxe/usr/include
                        
        LIBS += /home/user/mxe/usr/i686-w64-mingw32.static/lib/libsrc.a 
        TARGET = ../release/SimulIDE_0.1.4-Win32/SimulIDE_0.1.4
    } else { 
        TARGET = ../release/SimulIDE_0.1.4-Win32-NO_PIC/SimulIDE_0.1.4
        }
}
isEqual( _ARCH,"Win64") {
    Release:DESTDIR = ../release
    
    LIBS += ../include/simavr/lib/libsimavrW64.a
    
    INCLUDEPATH += ../include/simavr/sim
    
    isEqual( _PICC,"yes"){
        TARGET = ../release/SimulIDE_0.1.4-Win64/SimulIDE_0.1.4
    } else { 
        TARGET = ../release/SimulIDE_0.1.4-Win64-NO_PIC/SimulIDE_0.1.4
    }
}
