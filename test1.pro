#-------------------------------------------------
#
# Project created by QtCreator 2016-05-04T21:14:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test1
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    switch.cpp

HEADERS  += dialog.h \
    switch.h

FORMS    += dialog.ui

INCLUDEPATH += /usr/local/opencvARM/include/opencv \
               /usr/local/opencvARM/include/opencv2 \
               /usr/local/opencvARM/include

LIBS += /usr/local/opencvARM/lib/libopencv*
