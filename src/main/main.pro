QT -= gui

TARGET = main
CONFIG -= app_bundle

TEMPLATE = app

include(../VBE/VBE.pri)
include(../common.pri)

SOURCES += \
    src/main.cpp

OTHER_FILES += \
    ../../assets/quad.vert \
    ../../assets/quad.frag \
    ../../assets/cube.vert \
    ../../assets/cube.frag \
    ../../assets/awesomeface.png
