TEMPLATE = app

QT += core network
QT -= gui

CONFIG(release, debug|release):


DEFINES += QT_NO_DEBUG_OUTPUT
#CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

CONFIG += c++11

TARGET = Test_Fbr_2
CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp \
    client.cpp

HEADERS += \
    client.h
