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
    src/websocketinput.cpp \
    src/actor.cpp \
    src/player.cpp \
    src/scene.cpp \
    src/gamescene.cpp \
    src/groundactor.cpp \
    src/assets.cpp \
    src/webserver.cpp

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

HEADERS += \
    src/actor.h \
    src/player.h \
    src/scene.h \
    src/gamescene.h \
    src/groundactor.h \
    src/assets.h
