#-------------------------------------------------
#
# Project created by QtCreator 2018-04-29T15:09:07
#
#-------------------------------------------------

QT += core gui gamepad network widgets charts

TARGET = Drone
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    transmitter.cpp \
    preprocessor.cpp \
    realtimeplot.cpp \
    packet.cpp

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
