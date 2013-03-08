/*
 * msgSendRecv.cpp
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 6
 * parekhsm@vt.edu
 * 11/15/2012
 *
 */

#include "msgSendRecv.h"

msgSendRecv::msgSendRecv(QString newid, const char *newmid, QObject *parent)
{
    id = newid;
    mid = newmid;
    //qDebug() << mid;
    setParent(parent);
    openMailbox();
    NoC = 0;
}

msgSendRecv::~msgSendRecv()
{
    closeMailbox();
}

// opens a new mailbox, if its the server defaults to /mailbox0
void msgSendRecv::openMailbox()
{
    // DEBUG
    //qDebug() << "Opening Mailbox: ";

    attr.mq_maxmsg = 1;
    attr.mq_msgsize = MSGSZ;
    attr.mq_flags = 0;
    mq_unlink(mid);
    myMailbox = mq_open(mid, O_RDONLY | O_CREAT, S_IRWXU, &attr);

    // DEBUG
    //qDebug() << id << mid;
    int retCode = 0;
    if (id != "Server")
        retCode = connectMailboxToServer();
    // DEBUG
//    if (retCode)
//        qDebug() << "Connection Error";
//    else
//        qDebug() << "Mailbox opened" << myMailbox;
}

// Closes the mailbox opened
void msgSendRecv::closeMailbox()
{
    // DEBUG
    //qDebug() << "Closing Mailbox: ";
    mq_unlink(mid);
    mq_close(myMailbox);

    // DEBUG
    //qDebug() << "Mailbox closed";
}

// connects a client mailbox to the server mailbox
int msgSendRecv::connectMailboxToServer()
{
    serverBox = mq_open(SERVERBOX, O_RDWR);
    //qDebug() << "ServerBox:" << serverBox;
    if (serverBox < 0)
    {
        //qDebug() << "Server Error!";
        errorBox.setText("Server not started, client will not work, close client and start the server!");
        errorBox.exec();
        return -1;
    }
    else
        return 0;
}

// Server use:
// holds client addresses in an external array
void msgSendRecv::addClientMB(int pos)
{
    QString mail = "/mailbox";
    std::string addClient;
    mail.append(QString("%1").arg(pos));
    addClient = mail.toLatin1().constData();
    //qDebug() << "Adding Client:" << pos << addClient.c_str();
    external[pos-1] = mq_open(addClient.c_str(), O_RDWR);
    ++NoC;
    //qDebug() << "Number of Clients: " << NoC;

}

// Server Use:
// removes client address upon client exiting
void msgSendRecv::removeClientMB(int pos)
{
    QString mail = "/mailbox";
    std::string addClient;
    mail.append(QString("%1").arg(pos));
    addClient = mail.toLatin1().constData();
    //qDebug() << "Removing Client:" << pos << addClient.c_str();
    //mq_close(external[pos-1]);
	
	//qDebug() << NoC << pos;
	int i;
    for (i = pos; i < NoC; i++)
    {
	   	external[i] = external[i+1];
    }
	--NoC;
    external[i] = NULL;
    //qDebug() << "Number of Clients: " << NoC;

}


// Sends a message to specified recepient
void msgSendRecv::sendMsg(int recepient, QString message)
{
    int retCode;
    std::string buf;
    // protocol: sender|message
    message = id + "|" + message;
    // convert message to char*
    buf = message.toLatin1().constData();
    //qDebug() << buf.c_str() << recepient;
    // send it to server, else if its server sending messages out, send it to a mailbox
    // in external array
    if (recepient == 0)
        retCode = mq_send(serverBox, buf.c_str(), strlen(buf.c_str()), 0);
    else
    {
        //qDebug() << "pointer";
        retCode = mq_send(external[recepient-1], buf.c_str(), strlen(buf.c_str()), 0);
    }
    //qDebug() << retCode;
}

// receives message from its mailbox
void msgSendRecv::recvMsg()
{
    //qDebug() << "Receiving...";
    char buf[MSGSZ] = {'\0'};
    int msgVal;
    while(1)
    {
        msgVal = mq_receive(myMailbox, buf, MSGSZ, 0);
        //qDebug() << buf;
        // send message to top level after it has been received
        emit messageRecv(QString("%1").arg(buf));
        // clear buffer
        for (int i = 0; i < MSGSZ; i++)
        {
            buf[i] = '\0';
        }
    }
}

// starts reading the mailbox
void msgSendRecv::startRecv()
{
    recvMsg();
}
