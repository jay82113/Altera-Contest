#-------------------------------------------------
#
# Project created by QtCreator 2016-05-04T21:14:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = test1
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    switch.cpp \
    face_tracker.cpp \
    qcustomplot.cpp \
    stepdetection.cpp \
    ppgfilter.cpp \
    iir_filter.cpp \
    ppg_detection.cpp \
    ppgfilter_nomove.cpp

HEADERS  += dialog.h \
    switch.h \
    face_tracker.h \
    qcustomplot.h \
    stepdetection.h \
    ppgfilter.h \
    iir_filter.h \
    ppg_detection.h \
    ppgfilter_nomove.h

FORMS    += dialog.ui

INCLUDEPATH += /usr/local/opencv/include/opencv \
               /usr/local/opencv/include/opencv2 \
               /usr/local/opencv/include

LIBS += /usr/local/opencv/lib/libopencv*
