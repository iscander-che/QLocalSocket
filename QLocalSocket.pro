# Кроме модулей core gui подключаем network
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QLocalSocket
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mylocalsocket.cpp

HEADERS += \
        mylocalsocket.h
