TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


QMAKE_CXXFLAGS += -Wall -Wpedantic -Wconversion -Wunused-result # -Werror
SOURCES += main.cpp
