
VERSION     = "0.1.5"
TARGET_NAME = SimulIDE_$$VERSION-$$_ARCH$$_BITS

CONFIG(release, debug|release) {
        TARGET_PREFIX = $$BUILD_DIR/release/$$TARGET_NAME
        _OBJECTS_DIR  = $$OUT_PWD/build/release
}
CONFIG(debug, debug|release) {
        TARGET_PREFIX = $$BUILD_DIR/debug/$$TARGET_NAME
        _OBJECTS_DIR  = $$OUT_PWD/build/debug
}
OBJECTS_DIR *= $$_OBJECTS_DIR
MOC_DIR     *= $$_OBJECTS_DIR
INCLUDEPATH += $$OBJECTS_DIR

#mkpath($$OBJECTS_DIR)
mkpath($$TARGET_PREFIX/bin)
mkpath($$TARGET_PREFIX/lib/simulide/plugins)

# This required to build plugins in the same architecture
_var_list  = "VERSION="$$VERSION
_var_list += "_ARCH="$$_ARCH
_var_list += "_BITS="$$_BITS
_var_list += "TARGET_NAME="$$TARGET_NAME
_var_list += "TARGET_PREFIX="$$TARGET_PREFIX
_var_list += "QMAKE_EXEC="$$QMAKE_EXEC
_var_list += "_CROSS="$$_CROSS
write_file(version, _var_list)

TEMPLATE = app

isEqual( _ARCH,"Lin") {
    DEFINES += MAINMODULE_EXPORT=

    QMAKE_LIBS += -lutil
    QMAKE_LFLAGS += -Wl,-export-dynamic
    QT_EXTENSION_SHLIB="so"
}

isEqual( _ARCH,"Win") {
    CONFIG -= console
    CONFIG += windows
    CONFIG -= debug_and_release debug_and_release_target
    
    #Have to use #include <QtCore/QtGlobal> everywhere for Q_DECL_EXPORT?
    DEFINES += MAINMODULE_EXPORT=__declspec\\\(dllexport\\\)

    DEP_DIR=$$PWD/dependencies/build-$$_ARCH$$_BITS/lib
    mkpath( $$DEP_DIR )

    QMAKE_LFLAGS += -Wl,--exclude-all-symbols
    QMAKE_LFLAGS += -Wl,--out-implib,$$DEP_DIR/libsimulide.a
    QMAKE_LFLAGS += -Wl,-output-def,$$DEP_DIR/simulide.def
    QMAKE_LIBS += -lws2_32

    RC_ICONS += ../src/icons/simulide.ico

    QT_EXTENSION_SHLIB="dll"
}

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += QT_EXTENSION_SHLIB=$${QT_EXTENSION_SHLIB}

SOURCES += ../src/*.cpp \
    ../src/gui/*.cpp \
    ../src/gui/circuitwidget/*.cpp \
    ../src/gui/circuitwidget/components/*.cpp \
    ../src/gui/circuitwidget/components/mcu/*.cpp \
    ../src/gui/oscopewidget/*.cpp \
    ../src/gui/plotterwidget/*.cpp \
    ../src/gui/terminalwidget/*.cpp \
    ../src/gui/QPropertyEditor/*.cpp \
    ../src/gui/componentselector/*.cpp \
    ../src/simulator/*.cpp \
    ../src/simulator/elements/*.cpp \
    ../src/simulator/elements/processors/*.cpp \
    ../src/misc/simuapi_apppath.cpp

HEADERS += ../src/*.h \
    ../src/gui/*.h \
    ../src/gui/circuitwidget/*.h \
    ../src/gui/circuitwidget/components/*.h \
    ../src/gui/circuitwidget/components/mcu/*.h \
    ../src/gui/oscopewidget/*.h \
    ../src/gui/plotterwidget/*.h \
    ../src/gui/terminalwidget/*.h \
    ../src/gui/QPropertyEditor/*.h \
    ../src/gui/componentselector/*.h \
    ../src/simulator/*.h \
    ../src/simulator/elements/*.h \
    ../src/simulator/elements/processors/*.h \
    ../src/misc/simuapi_apppath.h

INCLUDEPATH += ../src \
    ../src/gui \
    ../src/gui/circuitwidget \
    ../src/gui/circuitwidget/components \
    ../src/gui/circuitwidget/components/mcu \
    ../src/gui/oscopewidget \
    ../src/gui/plotterwidget \
    ../src/gui/terminalwidget \
    ../src/gui/QPropertyEditor \
    ../src/gui/componentselector \
    ../src/gui/editorwidget \
    ../src/gui/editorwidget/findreplacedialog \
    ../src/simulator \
    ../src/simulator/elements \
    ../src/simulator/elements/processors \
    ../src/misc
    
FORMS += ../src/gui/componentselector/compplugin.ui

QMAKE_CXXFLAGS_DEBUG -= -O
QMAKE_CXXFLAGS_DEBUG -= -O1
QMAKE_CXXFLAGS_DEBUG -= -O2
QMAKE_CXXFLAGS_DEBUG -= -O3
QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CXXFLAGS += -Wno-unused-parameter

RESOURCES = ../src/application.qrc

QT += widgets
QT += xml
QT += concurrent

CONFIG += qt 
CONFIG += warn_on
CONFIG += no_qml_debug
CONFIG *= c++11

DESTDIR = $$TARGET_PREFIX/bin

TARGET = SimulIDE_$$VERSION

copy2dest.commands = \
$(MKDIR) $$TARGET_PREFIX/share/simulide/data ; \
$(MKDIR) $$TARGET_PREFIX/share/simulide/examples ; \
$(COPY_DIR) ../resources/data $$TARGET_PREFIX/share/simulide ; \
$(COPY_DIR) ../resources/examples $$TARGET_PREFIX/share/simulide

QMAKE_EXTRA_TARGETS += copy2dest
POST_TARGETDEPS += copy2dest

message( "-----------------------------" )
message( "    " $$TARGET_NAME )
message( "    TARGET_PREFIX=" $$TARGET_PREFIX )
message( "-----------------------------" )


