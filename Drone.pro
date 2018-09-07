#-------------------------------------------------
#
# Project created by QtCreator 2018-04-29T15:09:07
#
#-------------------------------------------------

QT += core gui gamepad network widgets charts 3dcore 3drender 3dinput 3dextras

TARGET = Drone
TEMPLATE = app


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    transmitter.cpp \
    preprocessor.cpp \
    packet.cpp \
    realtimeplot.cpp

HEADERS += \
        mainwindow.h \
    transmitter.h \
    packet.h \
    preprocessor.h \
    realtimeplot.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resources.qrc
