#-------------------------------------------------
#
# Project created by QtCreator 2017-04-29T23:05:00
#
#-------------------------------------------------


QT += core gui
QT += charts
CONFIG += c++14


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatabaseExpr
TEMPLATE = app

debug {
  DEFINES += __DEBUG__
}

HEADERS +=


UI_DIR = ./ui

DEFINES += __CLIENT__

SOURCES += \
    src/client/main.cpp \
    src/client/login.cpp \
    src/client/management.cpp

HEADERS += \
    src/client/login.h \
    src/client/management.h

FORMS    += \
    src/client/login.ui \
    src/client/management.ui








