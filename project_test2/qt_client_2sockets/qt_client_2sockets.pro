TEMPLATE = app

QT += core network
QT -= gui

#CONFIG(release, debug|release):
DEFINES += QT_NO_DEBUG_OUTPUT

CONFIG += c++11

CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp \
    clientreader.cpp \
    clientwriter.cpp

HEADERS += \
    clientreader.h \
    clientwriter.h \
    packet.h
