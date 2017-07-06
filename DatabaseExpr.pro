#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T23:05:00
#
#-------------------------------------------------


QT += core gui
QT += charts
QT += sql
CONFIG += c++14


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatabaseExpr
TEMPLATE = app

debug {
  DEFINES += __DEBUG__
}


UI_DIR = ./ui

SOURCES += \
    src/main.cpp \
    src/login.cpp \
    src/management.cpp \
    src/dispatch.cpp

HEADERS += \
    src/login.h \
    src/management.h \
    src/dispatch.h

FORMS    += \
    src/login.ui \
    src/management.ui








