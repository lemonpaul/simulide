


include( ../commonplugin.pro )

TARGET  = $$qtLibraryTarget(picplugin)


SOURCES =   piccomponent.cpp \
            piccomponentpin.cpp \
            picprocessor.cpp 
            
HEADERS =   piccomponent.h \
            piccomponentpin.h \
            picprocessor.h 

INCLUDEPATH +=  /usr/include/glib-2.0\
                        
LIBS += /usr/lib/libgpsim.so 

DESTDIR = PICplugin/PICplugin-$$_ARCH/data/plugins



isEqual( _ARCH,"Lin32") {

    INCLUDEPATH +=  /usr/lib/i386-linux-gnu/glib-2.0/include \
                    /usr/local/include/gpsim    

}
isEqual( _ARCH,"Lin64") {
    INCLUDEPATH +=  /usr/lib/x86_64-linux-gnu/glib-2.0/include \
                    /usr/local/include/gpsim

}
isEqual( _ARCH,"Win32") {
    DEFINES += Win 
    Release:DESTDIR = ../release
    
    QMAKE_LIBS += -lws2_32
    QMAKE_LIBS += -lpthread
        
        INCLUDEPATH +=  /home/user/mxe/usr/i686-w64-mingw32.static/include/glib-2.0 \
                        /home/user/mxe/usr/i686-w64-mingw32.static/lib/glib-2.0/include \
                        /home/user/mxe/usr/include
                        
        LIBS += /home/user/mxe/usr/i686-w64-mingw32.static/lib/libsrc.a 
}
isEqual( _ARCH,"Win64") {
}




