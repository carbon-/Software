#-------------------------------------------------
#
# Project created by QtCreator 2012-11-10T16:22:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chatServer
TEMPLATE = app


SOURCES += main.cpp\
        chatserver.cpp\
        ../chatClient/msgSendRecv.cpp

HEADERS  += chatserver.h \
    messages.h\
    ../chatClient/msgSendRecv.h

FORMS    += chatserver.ui
