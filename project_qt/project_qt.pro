TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt

QT += network core

SOURCES += main.cpp \
    client.cpp \
    a.cpp \
    worker.cpp

HEADERS += \
    client.h \
    a.h \
    worker.h
