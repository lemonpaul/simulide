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

RESOURCES = application.qrc

# TRANSLATIONS += SimulIDE.ts
QT += xml

CONFIG += qt \
    warn_on \
#    thread

LIBS += $$_PRO_FILE_PWD_/include/simavr/libsimavr.a


TARGET = SimulIDE/SimulIDE

INCLUDEPATH += build

OBJECTS_DIR = build

MOC_DIR = build

