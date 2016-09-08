TEMPLATE = app

QT += qml quick androidextras
CONFIG += c++11

SOURCES += main.cpp \
    image_signal.cpp \
    draw_image_item.cpp \
    rakvideo_wrapper.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    android-source/build.gradle \
    android-source/AndroidManifest.xml \
    android-source/res/values/libs.xml \
    android-source/src/org/mc/rak/RakSend.java \
    android-source/src/org/mc/rak/RakClient.java \
    android-source/src/org/mc/rak/RakVideo.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-source

HEADERS += \
    image_signal.h \
    draw_image_item.h \
    rakvideo_wrapper.h
