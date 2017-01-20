QT -= gui

TARGET = main
CONFIG -= app_bundle
CONFIG -= embed_manifest_exe

LIBS += -lboost_system -lboost_coroutine -lboost_context -lboost_thread -lcrypto

TEMPLATE = app

include(../VBE/VBE.pri)
include(../common.pri)

SOURCES += \
    src/main.cpp \
    src/websocketinput.cpp

OTHER_FILES += \
    ../../assets/awesomeface.png \
    ../../android/AndroidManifest.xml

RC_FILE = $$PWD/resources.rc

DISTFILES += \
    resources.rc \
    manifest.xml \
    ../../assets/quad2.frag \
    ../../client.html \
    ../../assets/ground.frag \
    ../../assets/ground.vert \
    ../../assets/quad.frag \
    ../../assets/quad.vert
