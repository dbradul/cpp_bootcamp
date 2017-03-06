TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


win32 {
#      BOOSTDIR = C:/...
#      INCLUDEPATH += \
#         $(BOOSTDIR)
      #LIBS += -L...
}

LIBS +=  -lboost_system \
         -lboost_coroutine \
         -lboost_timer \
         -lboost_thread

LIBS += -pthread

SOURCES += main.cpp
