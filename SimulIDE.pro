 ###########################################################################
 #   Copyright (C) 2012   by santiago González                             #
 #   santigoro@gmail.com                                                   #
 #                                                                         #
 #   This program is free software; you can redistribute it and/or modify  #
 #   it under the terms of the GNU General Public License as published by  #
 #   the Free Software Foundation; either version 3 of the License, or     #
 #   (at your option) any later version.                                   #
 #                                                                         #
 #   This program is distributed in the hope that it will be useful,       #
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of        #
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
 #   GNU General Public License for more details.                          #
 #                                                                         #
 #   You should have received a copy of the GNU General Public License     #
 #   along with this program; if not, see <http://www.gnu.org/licenses/>.  #
 #                                                                         #
 ###########################################################################
 
VERSION     = "0.3.11"

TEMPLATE = app

QT += svg
QT += xml
QT += script
QT += widgets
QT += concurrent
QT += serialport
QT += multimedia widgets

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
    ../src/gui/filebrowser/*.cpp \
    ../src/gui/editorwidget/*.cpp \
    ../src/gui/editorwidget/findreplacedialog/*.cpp \
    ../src/gui/serialporwidget/*.cpp \
    ../src/simulator/*.cpp \
    ../src/simulator/elements/*.cpp \
    ../src/simulator/elements/processors/*.cpp \
    ../src/simavr/sim/*.c \
    ../src/simavr/cores/*.c \
    ../src/gpsim/*.cc \
    ../src/gpsim/devices/*.cc \
    ../src/gpsim/modules/*.cc \
    ../src/gpsim/registers/*.cc

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
    ../src/gui/filebrowser/*.h \
    ../src/gui/editorwidget/*.h \
    ../src/gui/editorwidget/findreplacedialog/*.h \
    ../src/gui/serialporwidget/*.h \
    ../src/simulator/*.h \
    ../src/simulator/elements/*.h \
    ../src/simulator/elements/processors/*.h \
    ../src/simavr/sim/*.h \
    ../src/simavr/sim/avr/*.h  \
    ../src/simavr/cores/*.h \
    ../resources/data/*.xml \
    ../src/gpsim/*.h \
    ../src/gpsim/devices/*.h \
    ../src/gpsim/modules/*.h \
    ../src/gpsim/registers/*.h

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
    ../src/gui/filebrowser \
    ../src/gui/editorwidget \
    ../src/gui/editorwidget/findreplacedialog \
    ../src/gui/serialporwidget \
    ../src/simulator \
    ../src/simulator/elements \
    ../src/simulator/elements/processors \
    ../src/simavr \
    ../src/simavr/sim \
    ../src/simavr/sim/avr \
    ../src/simavr/cores \
    ../src/gpsim \
    ../src/gpsim/devices \
    ../src/gpsim/modules \
    ../src/gpsim/registers

TRANSLATIONS +=  \
    ../resources/translations/simulide.ts \
    ../resources/translations/simulide_en.ts \
    ../resources/translations/simulide_es.ts \
    ../resources/translations/simulide_ru.ts 

RESOURCES = ../src/application.qrc


QMAKE_CXXFLAGS_DEBUG -= -O
QMAKE_CXXFLAGS_DEBUG -= -O1
QMAKE_CXXFLAGS_DEBUG -= -O2
QMAKE_CXXFLAGS_DEBUG -= -O3
QMAKE_CXXFLAGS_DEBUG += -O3
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-missing-field-initializers

QMAKE_CFLAGS += --std=gnu11
QMAKE_CFLAGS += -Wno-unused-result
QMAKE_CFLAGS += -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-missing-field-initializers
QMAKE_CFLAGS += -Wno-implicit-function-declaration
QMAKE_CFLAGS += -Wno-int-conversion
QMAKE_CFLAGS += -Wno-sign-compare
QMAKE_CFLAGS += -O2
QMAKE_CFLAGS += -fPIC

QMAKE_LIBS += -lelf

CONFIG += qt 
CONFIG += warn_on
CONFIG += no_qml_debug
CONFIG *= c++11

DEFINES += MAINMODULE_EXPORT=
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TARGET_NAME = SimulIDE_$$VERSION$$

CONFIG( release, debug|release ) {
        TARGET_PREFIX = $$BUILD_DIR/release/$$TARGET_NAME
        _OBJECTS_DIR  = $$OUT_PWD/build/release
}

CONFIG( debug, debug|release ) {
        TARGET_PREFIX = $$BUILD_DIR/debug/$$TARGET_NAME
        _OBJECTS_DIR  = $$OUT_PWD/build/debug
}

OBJECTS_DIR *= $$_OBJECTS_DIR
MOC_DIR     *= $$_OBJECTS_DIR
INCLUDEPATH += $$OBJECTS_DIR

DESTDIR = $$TARGET_PREFIX/bin

TARGET = simulide

mkpath( $$TARGET_PREFIX/bin )


runLrelease.commands = lrelease ../resources/translations/*.ts; 
QMAKE_EXTRA_TARGETS += runLrelease
POST_TARGETDEPS     += runLrelease

copy2dest.commands = \
$(MKDIR)    $$TARGET_PREFIX/share/simulide/data ; \
$(MKDIR)    $$TARGET_PREFIX/share/simulide/examples ; \
$(MKDIR)    $$TARGET_PREFIX/share/simulide/translations ; \
$(COPY_DIR) ../resources/data              $$TARGET_PREFIX/share/simulide ; \
$(COPY_DIR) ../resources/examples          $$TARGET_PREFIX/share/simulide ; \
$(COPY_DIR) ../resources/icons             $$TARGET_PREFIX/share ; \
$(MOVE)     ../resources/translations/*.qm $$TARGET_PREFIX/share/simulide/translations ;

QMAKE_EXTRA_TARGETS += copy2dest
POST_TARGETDEPS     += copy2dest


message( "-----------------------------" )
message( "    " $$TARGET_NAME )
message( "    TARGET_PREFIX=" $$TARGET_PREFIX )
message( "-----------------------------" )

