TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wall -pedantic

SOURCES += main.cpp \
    logger.cpp \
    a.cpp

HEADERS += \
    logger.h \
    a.h
