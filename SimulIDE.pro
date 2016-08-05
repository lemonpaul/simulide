
_ARCH = Lin32
_PICC = no





TEMPLATE = app

SOURCES += src/*.cpp \
    src/gui/*.cpp \
    src/gui/circuitwidget/*.cpp \
    src/gui/circuitwidget/components/*.cpp \
    src/gui/circuitwidget/components/mcu/*.cpp \
    src/gui/oscopewidget/*.cpp \
    src/gui/plotterwidget/*.cpp \
    src/gui/QPropertyEditor/*.cpp \
    src/simulator/*.cpp \
    src/simulator/elements/*.cpp \
    src/simulator/elements/processors/*.cpp \

HEADERS += src/*.h \
    src/gui/*.h \
    src/gui/circuitwidget/*.h \
    src/gui/circuitwidget/components/*.h \
    src/gui/circuitwidget/components/mcu/*.h \
    src/gui/oscopewidget/*.h \
    src/gui/plotterwidget/*.h \
    src/gui/QPropertyEditor/*.h \
    src/simulator/*.h \
    src/simulator/elements/*.h \
    src/simulator/elements/processors/*.h \

INCLUDEPATH += src \
    src/gui \
    src/gui/circuitwidget \
    src/gui/circuitwidget/components \
    src/gui/circuitwidget/components/mcu \
    src/gui/oscopewidget \
    src/gui/plotterwidget \
    src/gui/QPropertyEditor \
    src/simulator \
    src/simulator/elements \
    src/simulator/elements/processors \
    include/simavr/sim \
    #/usr/local/include/gpsim\
    #/usr/include/glib-2.0\
    #/usr/lib/i386-linux-gnu/glib-2.0/include\
    #/usr/lib/x86_64-linux-gnu/glib-2.0/include \
    
    
    
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CXXFLAGS += -Wno-unused-parameter 

RESOURCES = application.qrc
# TRANSLATIONS += SimulIDE.ts

QT += concurrent
QT += widgets
QT += xml
QT += gui

CONFIG += qt \
    warn_on \
#    debug

message( $$_ARCH $$_PICC)

INCLUDEPATH += build
OBJECTS_DIR  = build
MOC_DIR      = build

isEqual( _PICC,"no") { 
    DEFINES += NO_PIC 
}
isEqual( _ARCH,"Lin32") {
    LIBS += $$_PRO_FILE_PWD_/include/simavr/libsimavrL32.a
    
    isEqual( _PICC,"yes") {
        TARGET = release/SimulIDE_0.0.3-Lin32/SimulIDE_0.0.3
        INCLUDEPATH +=  /usr/include/glib-2.0\
                        /usr/lib/i386-linux-gnu/glib-2.0/include
                        
        LIBS += /usr/lib/libgpsim.so 
    } else { 
        TARGET = release/SimulIDE_0.0.3-Lin32-NO_PIC/SimulIDE_0.0.3
    }
}
isEqual( _ARCH,"Lin64") {
    LIBS += $$_PRO_FILE_PWD_/include/simavr/libsimavrL64.a
    
    isEqual( _PICC,"yes"){ 
        TARGET = release/SimulIDE_0.0.3-Lin64/SimulIDE_0.0.3
        INCLUDEPATH +=  /usr/include/glib-2.0\
                        /usr/lib/x86_64-linux-gnu/glib-2.0/include
                        
        LIBS += /usr/lib/libgpsim.so 
    }else { 
        TARGET = release/SimulIDE_0.0.3-Lin64-NO_PIC/SimulIDE_0.0.3
    }
}
isEqual( _ARCH,"Win32") {
    LIBS += $$_PRO_FILE_PWD_/include/simavr/libsimavrW32.a
    
    isEqual( _PICC,"yes"){
        
        INCLUDEPATH +=  /home/user/mxe/usr/i686-w64-mingw32.static/include/glib-2.0 \
                        /home/user/mxe/usr/i686-w64-mingw32.static/lib/glib-2.0/include \
                        /home/user/mxe/usr/include
                        
        LIBS += /home/user/mxe/usr/i686-w64-mingw32.static/lib/libsrc.a 
        TARGET = release/SimulIDE_0.0.3-Win32/SimulIDE_0.0.3
    } else { 
        TARGET = release/SimulIDE_0.0.3-Win32-NO_PIC/SimulIDE_0.0.3
        }
}
isEqual( _ARCH,"Win64") {
    LIBS += $$_PRO_FILE_PWD_/include/simavr/libsimavrW64.a
    
    isEqual( _PICC,"yes"){
        TARGET = release/SimulIDE_0.0.3-Win64/SimulIDE_0.0.3
    } else { 
        TARGET = release/SimulIDE_0.0.3-Win64-NO_PIC/SimulIDE_0.0.3
    }
}
