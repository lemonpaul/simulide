TEMPLATE = app

SOURCES += src/*.cpp \
    src/gui/*.cpp \
    src/gui/circuitwidget/*.cpp \
    src/gui/circuitwidget/components/*.cpp \
    src/gui/circuitwidget/components/mcu/*.cpp \
    src/gui/oscopewidget/*.cpp \
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
    src/gui/QPropertyEditor \
    src/simulator \
    src/simulator/elements \
    src/simulator/elements/processors \
    include \
    include/simavr/sim \
    #C:/MinGW/lib\
    #/usr/include/glib-2.0\
    #/usr/lib/i386-linux-gnu/glib-2.0/include\
    
    
    
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CXXFLAGS += -Wno-unused-parameter
#QMAKE_CFLAGS += -std=c99

RESOURCES = application.qrc

# TRANSLATIONS += SimulIDE.ts
QT += xml
QT += widgets
QT += gui

CONFIG += qt \
    warn_on 
    thread
DEFINES += NO_PIC

LIBS += $$_PRO_FILE_PWD_/include/simavr/obj-x86_64-linux-gnu/libsimavr.a
LIBS += /usr/lib/x86_64-linux-gnu/libutil.a

#LIBS += /usr/lib/libgpsim.so.0

TARGET = SimulIDE/SimulIDE

INCLUDEPATH += build

OBJECTS_DIR = build

MOC_DIR = build

