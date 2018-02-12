TEMPLATE = lib

CONFIG  += plugin
CONFIG  += qt
#CONFIG -= debug_and_release debug_and_release_target
#CONFIG += release
CONFIG *= c++11

QT += xml
QT += widgets
#QT += gui
QT += concurrent


include( ../version )

isEmpty(TARGET_PREFIX) {
    error("Please run qmake first on the main executeable project in one of build_XXX folders")
}

isEqual( _ARCH,"Lin") {

    DEFINES += MAINMODULE_EXPORT=

}
isEqual( _ARCH,"Win") {

    DEFINES += MAINMODULE_EXPORT=__declspec\\\(dllimport\\\)
#Q_DECL_IMPORT
    QMAKE_LIBS += -lsimulide
    CONFIG -= console
    CONFIG += windows
    CONFIG += dll
}

CONFIG(release, debug|release) {
        _OBJECTS_DIR  = $$OUT_PWD/build/release
}
CONFIG(debug, debug|release) {
        _OBJECTS_DIR  = $$OUT_PWD/build/debug
}
OBJECTS_DIR *= $$_OBJECTS_DIR
MOC_DIR     *= $$_OBJECTS_DIR
INCLUDEPATH += $$OBJECTS_DIR
mkpath($$OBJECTS_DIR)

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
#                ../../include/simavr/sim \
                
DESTDIR = $$TARGET_PREFIX/lib/simulide/plugins

QMAKE_LFLAGS += "-L$${TARGET_PREFIX}/lib"




#DEFINES += MYSHAREDLIB_LIBRARY

QMAKE_CFLAGS_SHLIB += -fpic

QMAKE_CFLAGS_DEBUG += -O0
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

#copy2dest.commands = $(MKDIR) \"$$shell_path($$TARGET_PREFIX/share/simulide/data)\" & \
#    $(MKDIR) \"$$shell_path($$TARGET_PREFIX/share/simulide/examples)\" &
copy2dest.commands += $$copyToDestdir($$PLGN_DIR_DATA $$PLGN_DIR_EXAMPLES)

QMAKE_EXTRA_TARGETS += copy2dest
POST_TARGETDEPS += copy2dest

message( "-----------------------------" )
message( "       " $$VERSION $$_ARCH $$_BITS )
message( "-----------------------------" )
