#ifndef CHATTER_H
#define CHATTER_H

/*
 * chatter.h
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 6
 * parekhsm@vt.edu
 * 11/15/2012
 *
 */

#include <QtGui>
#include <QObject>
#include <QThread>
#include <QDebug>
#include "chatclient.h"

class chatter : public QObject
{
    Q_OBJECT

public:
    chatter(int id);                            // constructor
    ~chatter();                                 // destructor
private:
    chatClient* chatty;                         // chat client instance

signals:

protected:
    void setUpChatClient(int id);               // sets up a chat client
};

#endif // CHATTER_H
