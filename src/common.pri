QMAKE_CXXFLAGS += -std=c++0x

win32 {
    INCLUDEPATH += $$PWD/../libs/include
    LIBPATH     += $$PWD/../libs/lib

    LIBS += -static -lmingw32 -lSDL2 -lGLEW -lglu32 -lopengl32 -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc -static-libstdc++
    QMAKE_CXXFLAGS += -DWINDOWS -DGLEW_STATIC
    QMAKE_CFLAGS += -DWINDOWS -DGLEW_STATIC
}
else {
    LIBS += -lSDL2 -lGL -lGLEW -lBox2D
}
