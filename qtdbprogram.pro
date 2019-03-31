QT       += core gui widgets

TARGET   = qtdbprogram
TEMPLATE = app

DEFINES  += QT_DEPRECATED_WARNINGS

CONFIG   += c++11

SOURCES  += \
         main.cpp \
         mainwindow.cpp

HEADERS  += \
         mainwindow.h

FORMS    += \
         mainwindow.ui

RESOURCES += \
    translations.qrc
