QT -= gui

TARGET = main
CONFIG -= app_bundle
CONFIG -= embed_manifest_exe

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
    ../../assets/awesomeface.png \
    ../../android/AndroidManifest.xml

RC_FILE = $$PWD/resources.rc

DISTFILES += \
    resources.rc \
    manifest.xml \
    ../../assets/quad2.frag
