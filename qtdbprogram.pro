QT        += core gui widgets sql

TARGET    = qtdbprogram
TEMPLATE  = app

DEFINES   += QT_DEPRECATED_WARNINGS

CONFIG    += c++11

SOURCES   += \
          main.cpp \
          mainwindow.cpp \
          dbrequest.cpp

HEADERS   += \
          mainwindow.h \
          dbrequest.h

FORMS     += \
          mainwindow.ui

RESOURCES += \
          translations.qrc

pvs_studio.output = true
pvs_studio.cxxflags = -std=c++11
pvs_studio.sources = $${SOURCES}
include(../PVS-Studio.pri)
