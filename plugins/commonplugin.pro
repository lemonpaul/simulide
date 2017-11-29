TEMPLATE = lib

CONFIG += plugin
CONFIG += qt
CONFIG *= c++11

QT += xml
QT += widgets
QT += concurrent

include( ../version )

MAKEFILE = build_$$_ARCH$$_BITS/Makefile

isEmpty(TARGET_PREFIX) {
    error("Please run qmake first on the main executeable project in one of build_XXX folders")
}

isEqual( _ARCH,"Lin") {

    DEFINES += MAINMODULE_EXPORT=

}
isEqual( _ARCH,"Win") {

    DEFINES += MAINMODULE_EXPORT=__declspec\\\(dllimport\\\)

    LIBS += ../../../dependencies/build-$$_ARCH$$_BITS/lib/libsimulide.a
    
    CONFIG -= debug_and_release debug_and_release_target
    CONFIG -= console
    CONFIG += windows
    CONFIG += dll

    copy2dest.commands += $(COPY_DIR) $$PLGN_DIR_BIN $$TARGET_PREFIX & 
}

CONFIG(release, debug|release) {
        _OBJECTS_DIR  = $$OUT_PWD/release
        QMAKE_PRE_LINK +=$(DEL_DIR) ../release && 
}
CONFIG(debug, debug|release) {
        _OBJECTS_DIR  = $$OUT_PWD/debug
        QMAKE_PRE_LINK += $(DEL_DIR) ../debug &&
}

OBJECTS_DIR *= $$OUT_PWD/release
MOC_DIR     *= $$OBJECTS_DIR
INCLUDEPATH += $$OBJECTS_DIR
#mkpath( build_$$_ARCH$$_BITS/release )

INCLUDEPATH +=  ../../src \
                ../../src/gui \
                ../../src/gui/circuitwidget \
                ../../src/gui/circuitwidget/components \
                ../../src/gui/circuitwidget/components/mcu \
                ../../src/gui/oscopewidget \
                ../../src/gui/plotterwidget \
                ../../src/gui/terminalwidget \
                ../../src/gui/QPropertyEditor \
                ../../src/gui/componentselector \
                ../../src/simulator \
                ../../src/simulator/elements \
                ../../src/simulator/elements/processors \
                ../../src/misc \
                $${TARGET_PREFIX}/include
                
DESTDIR = $$TARGET_PREFIX/lib/simulide/plugins

QMAKE_LFLAGS += "-L../../dependencies/build-$$_ARCH$$_BITS/lib"

QMAKE_CFLAGS_SHLIB += -fpic
QMAKE_CFLAGS_DEBUG += -O0
QMAKE_CFLAGS += -std=c11 
QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

ComSpec=$$(ComSpec)

defineReplace(copyToDestdir) {
    folders = $$1
    copyList =

    for(folder, folders) {
        exists($$folder) {
            dest_path="$$TARGET_PREFIX/share/simulide"
            folder_name=$$basename(folder)
            !isEmpty(ComSpec) {
                dest_path="$$dest_path/$$folder_name"
            }
            copyList += $(MKDIR) \"$$shell_path($$TARGET_PREFIX/share/simulide/$$folder_name)\" &
            copyList += $(COPY_DIR) \"$$shell_path($$folder)\" \"$$shell_path($$dest_path)\" &
        }
    }
    return($$copyList)
}

#PLGN_DIR_DATA = $$PWD/resources/data
#PLGN_DIR_EXAMPLES = $$PWD/resources/examples

copy2dest.commands += $$copyToDestdir($$PLGN_DIR_DATA $$PLGN_DIR_EXAMPLES)

QMAKE_EXTRA_TARGETS += copy2dest
POST_TARGETDEPS += copy2dest

QMAKE_PRE_LINK += $(DEL_FILE) ../*.rc

message( "------  COMMON PLUGIN  -------" )
message( "       " $$VERSION $$_ARCH $$_BITS )
message( "-----------------------------" )
