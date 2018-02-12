#message("PWD="$$PWD)
message("BUILD_DIR="$$BUILD_DIR)

include( version )

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
mkpath($$OBJECTS_DIR)
mkpath($$TARGET_PREFIX/bin)
mkpath($$TARGET_PREFIX/lib/simulide/plugins)
mkpath($$TARGET_PREFIX/include)
mkpath($$TARGET_PREFIX/share/simulide)
#mkpath($$TARGET_PREFIX/share/simulide/data)
#mkpath($$TARGET_PREFIX/share/simulide/examples)

# This required to build plugins in the same architecture
#if (!infile(version, _ARCH, $$_ARCH) | !infile(version, _BITS, $$_BITS) | !infile(version, _PICC, $$_PICC) | !infile(version, TARGET_NAME, $$TARGET_NAME)) {
if (!infile(version, _ARCH, $$_ARCH) | !infile(version, _BITS, $$_BITS) | !infile(version, TARGET_NAME, $$TARGET_NAME)) {
        _var_list = "VERSION="$$VERSION
	_var_list += "_ARCH="$$_ARCH
	_var_list += "_BITS="$$_BITS
#	_var_list += "_PICC="$$_PICC
	_var_list += "TARGET_NAME="$$TARGET_NAME
        _var_list += "TARGET_PREFIX="$$TARGET_PREFIX
        write_file(version, _var_list)
}


TEMPLATE = app

QT_EXTENSION_SHLIB=$${QMAKE_EXTENSION_SHLIB}

isEqual( _ARCH,"Lin") {

    DEFINES += MAINMODULE_EXPORT=

    QMAKE_LIBS += -lutil
    QMAKE_LFLAGS += -Wl,-export-dynamic
    isEmpty(QT_EXTENSION_SHLIB) {
        QT_EXTENSION_SHLIB="so"
    }
}

isEqual( _ARCH,"Win") {

    #TEMPLATE = lib
    CONFIG -= console
    CONFIG += windows
    #Have to use #include <QtCore/QtGlobal> everywhere for Q_DECL_EXPORT?
    DEFINES += MAINMODULE_EXPORT=__declspec\\\(dllexport\\\)

    QMAKE_LFLAGS += -Wl,--exclude-all-symbols
    QMAKE_LFLAGS += -Wl,--out-implib,$$TARGET_PREFIX/lib/libsimulide.a
    QMAKE_LFLAGS += -Wl,-output-def,$$TARGET_PREFIX/lib/simulide.def
    QMAKE_LIBS += -lws2_32

    RC_ICONS += ../src/icons/simulide.ico

    isEmpty(QT_EXTENSION_SHLIB) {
        QT_EXTENSION_SHLIB="dll"
    }
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

#    ../src/gui/editorwidget/*.cpp \
#    ../src/gui/editorwidget/findreplacedialog/*.cpp \

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

#    ../src/gui/editorwidget/*.h \
#    ../src/gui/editorwidget/findreplacedialog/*.h \

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
#    ../include/simavr/sim \
    
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
# TRANSLATIONS += SimulIDE.ts

QT += widgets
QT += xml
#QT += gui
QT += concurrent

CONFIG += qt \
    warn_on

CONFIG += no_qml_debug
CONFIG *= c++11


#CONFIG += static

#CONFIG -= debug_and_release debug_and_release_target

#QMAKE_CXXFLAGS += -std=gnu++98
DESTDIR = $$TARGET_PREFIX/bin
#TARGET = $$TARGET_PREFIX/bin/SimulIDE_$$VERSION
TARGET = SimulIDE_$$VERSION

#$(MKDIR) \"$$shell_path($$TARGET_PREFIX/share/simulide/data)\" 2>&1 > copy2dest.log & \
#    $(MKDIR) \"$$shell_path($$TARGET_PREFIX/share/simulide/examples)\" 2>&1 >> copy2dest.log & \
#    $(COPY_DIR) \"$$shell_path(../release/examples)\" \"$$shell_path($$TARGET_PREFIX/share/simulide)\" 2>&1 >> copy2dest.log
#copy2dest_data.target = $$relative_path("$$TARGET_PREFIX/share/simulide/data/ic74.xml", $$OUT_PWD)
#copy2dest_data.commands = $(COPY_DIR) \"$$shell_path(../release/data)\" \"$$shell_path($$TARGET_PREFIX/share/simulide)\" > copy2dest.log 2>&1
#QMAKE_EXTRA_TARGETS += copy2dest_data
#POST_TARGETDEPS += $$copy2dest_data.target

ComSpec=$$(ComSpec)
isEmpty(ComSpec) {
    copy2dest.commands = $(MKDIR) $$TARGET_PREFIX/share/simulide/data ; \
     $(MKDIR) $$TARGET_PREFIX/share/simulide/examples ; \
     $(COPY_DIR) ../resources/data $$TARGET_PREFIX/share/simulide ; \
     $(COPY_DIR) ../resources/examples $$TARGET_PREFIX/share/simulide
} else {
    copy2dest.commands = $(COPY_DIR) \"$$shell_path(../resources)\" \"$$shell_path($$TARGET_PREFIX/share/simulide)\" > copy2dest.log 2>&1
}


QMAKE_EXTRA_TARGETS += copy2dest
POST_TARGETDEPS += copy2dest


message( "-----------------------------" )
message( "    " $$TARGET_NAME )
message( "    TARGET_PREFIX=" $$TARGET_PREFIX )
message( "-----------------------------" )


