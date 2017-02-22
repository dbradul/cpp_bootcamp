TEMPLATE = app

QT += core network
QT -= gui

CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

CONFIG += c++11

TARGET = qt_client_1thread
CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp \
    client.cpp

HEADERS += \
    client.h
