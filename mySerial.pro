#-------------------------------------------------
#
# Project created by QtCreator 2013-09-19T13:21:20
#
#-------------------------------------------------

QT       += core gui serialport  testlib
QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = mySerial
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
