#-------------------------------------------------
#
# Project created by QtCreator 2012-10-12T22:11:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = compSim
TEMPLATE = app


SOURCES += main.cpp\
        compgui.cpp \
    memory.cpp \
    control.cpp \
    cache.cpp

HEADERS  += compgui.h \
    memory.h \
    control.h \
    cache.h

FORMS    += compgui.ui
