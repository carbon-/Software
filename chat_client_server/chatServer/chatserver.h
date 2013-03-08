/*
 * chatserver.h
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 6
 * parekhsm@vt.edu
 * 11/15/2012
 *
 */
#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QMainWindow>
#include <QString>
#include <QMutex>
#include <QDebug>
#include <QThread>
#include <QRegExp>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "messages.h"
#include "../chatClient/msgSendRecv.h"


namespace Ui {
class chatServer;
}

class chatServer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit chatServer(QWidget *parent = 0);
    ~chatServer();


signals:
    void started();      // let clients know that they connected to server
    void ended();        // let clients know they have lost connection to server

protected slots:
    void exitApp();                 // exits the app
    void displayMsg(QString);       // displays events on server text edit

private:
    Ui::chatServer *ui;
    QThread* thread;
    msgSendRecv *server;
    QString buffer;
    QStringList clients;
    int numOfClients;
};

#endif // CHATSERVER_H
