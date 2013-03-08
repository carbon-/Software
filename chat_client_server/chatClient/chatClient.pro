#-------------------------------------------------
#
# Project created by QtCreator 2012-11-14T01:30:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chatClient
TEMPLATE = app


SOURCES += main.cpp\
        chatclient.cpp \
    msgSendRecv.cpp \
    chatter.cpp

HEADERS  += chatclient.h \
    msgSendRecv.h \
    chatter.h

FORMS    += chatclient.ui
