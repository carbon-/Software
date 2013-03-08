/*
 * msgSendRecv.h
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 6
 * parekhsm@vt.edu
 * 11/15/2012
 *
 */
#ifndef MSGSENDRECV_H
#define MSGSENDRECV_H
#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QMessageBox>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../chatServer/messages.h"

class msgSendRecv : public QObject
{
    Q_OBJECT
public:
    msgSendRecv(QString newid, const char* newmid, QObject *parent = 0);
    ~msgSendRecv();
    void openMailbox();                             // opens a mailbox
    void closeMailbox();                            // closes a mailbox
    int connectMailboxToServer();                   // connects a mailbox to server
    void sendMsg(int recepient, QString message);   // sends a message to another mailbox
    void recvMsg();                                 // receives a message from another mailbox
    void addClientMB(int pos);                      // server use: adds a client mailbox
    void removeClientMB(int pos);                   // server use: removes a client mailbox
public slots:
    void startRecv();                               // starts reading mailbox

signals:
    void messageRecv(QString);                      // updates the server/client with the received message

private:
    // id (name client connected as)
    QString id;
    // mailbox id (defined in messages.h)
    const char *mid;
    // mailbox
    mqd_t myMailbox;
    // serverbox
    mqd_t serverBox;
    // external for server use
    mqd_t external[MAXCLIENTS];
    // attribute struct
    struct mq_attr attr;
    // server error box
    QMessageBox errorBox;
    // number of clients
    int NoC;
};

#endif // MSGSENDRECV_H
