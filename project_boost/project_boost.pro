TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


BOOSTDIR = /usr/local/include/boost

INCLUDEPATH += \
   $(BOOSTDIR)

SOURCES += main.cpp
