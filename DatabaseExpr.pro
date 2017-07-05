#-------------------------------------------------
#
# Project created by QtCreator 2017-04-29T23:05:00
#
#-------------------------------------------------


Client {
    QT += core gui
    QT += charts
    CONFIG += c++14
}

Server {
    CONFIG += console c++14
    CONFIG -= app_bundle
    CONFIG -= qt
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatabaseExpr
TEMPLATE = app

debug {
  DEFINES += __DEBUG__
}

HEADERS += \
        src/socket.h \
        src/protocol.h \
        src/include/json.hpp \


UI_DIR = ./ui

Client{

    DEFINES += __CLIENT__

    SOURCES += \
        src/client/client.cpp \
        src/client/main.cpp \
        src/client/login.cpp \
        src/client/management.cpp

    HEADERS += \
        src/client/client.h \
        src/client/login.h \
        src/client/management.h

    FORMS    += \
        src/client/login.ui \
        src/client/management.ui
}

Server{
    DEFINES += __SERVER__

    SOURCES += \
        src/server/dispatch.cpp \
        src/server/server.cpp \
        src/server/main.cpp


    HEADERS += \
        src/server/dispatch.h \
        src/server/server.h \

}






