TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt

QT += network core

SOURCES += main.cpp \
    server.cpp \
    loopbackconnection.cpp

HEADERS += \
    server.h \
    loopbackconnection.h
