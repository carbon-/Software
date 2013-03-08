/*
 * chatserver.cpp
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 6
 * parekhsm@vt.edu
 * 11/15/2012
 *
 */
#include "chatserver.h"
#include "ui_chatserver.h"

chatServer::chatServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chatServer)
{
    ui->setupUi(this);
    numOfClients = 0;

    // connect gui to app
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(exitApp()));

    // create a mailbox for the server
    thread = new QThread();
    const char *mid = "/mailbox0";
    QString id = "Server";
    server = new msgSendRecv(id, mid);
    connect(this, SIGNAL(started()), server, SLOT(startRecv()));
    connect(server, SIGNAL(messageRecv(QString)), this, SLOT(displayMsg(QString)));
    server->moveToThread(thread);
    thread->start();
    // start reading the mailbox
    emit started();
}

chatServer::~chatServer()
{
    delete ui;
}

// Exits the app
void chatServer::exitApp()
{
    server->closeMailbox();
    qApp->exit();
}

// Displays message from mailbox to TextEdit
void chatServer::displayMsg(QString msg)
{
    QStringList messages;
    // protocol: Server|username: message
    messages = msg.split(QRegExp("\\|"), QString::SkipEmptyParts);
    QString dispMsg = messages.at(0) + ": " + messages.at(1);
    // if message is remove, client has left, update client list
    // paint event on TextEdit
    if (messages.at(1) == "Remove")
    {
        int pos;
        pos = clients.indexOf(messages.at(0));
        clients.removeAt(pos);
        --numOfClients;
        server->removeClientMB(pos);
        ui->plainTextEdit->appendPlainText(QString("Client %1: has left the chat session!").arg(messages.at(0)));
    }
    // if message is add, client has been added, update client list
    // paint event on TextEdit
    else if (messages.at(1) == "Add")
    {
        if (!clients.contains(messages.at(0)))
        {
            clients.append(messages.at(0));
            ++numOfClients;
            //qDebug() << numOfClients;
            server->addClientMB(numOfClients);
            ui->plainTextEdit->appendPlainText(QString("Client %1: has joined the chat session!").arg(messages.at(0)));
        }
    }
    // else update all clients with the message
    else
    {
        ui->plainTextEdit->appendPlainText(dispMsg);
        int numClients = clients.size();
        //qDebug() << numClients;
        for (int i = 1; i <= numClients; i++)
        {
            //qDebug() << i;
            server->sendMsg(i, dispMsg);
        }
    }

    //qDebug() << clients;
}
