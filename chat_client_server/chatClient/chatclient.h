#ifndef CHATCLIENT_H
#define CHATCLIENT_H

/*
 * chatclient.h
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 6
 * parekhsm@vt.edu
 * 11/15/2012
 *
 *
 * Top Level Class to handle the chat clients.
 */

#include <QWidget>
#include <QInputDialog>
#include <QThread>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include "msgSendRecv.h"
#include "../chatServer/messages.h"

namespace Ui {
class chatClient;
}

class chatClient : public QWidget
{
    Q_OBJECT
    
public:
    explicit chatClient(int id, QWidget *parent = 0);           // constructor
    ~chatClient();                                              // destructor

protected slots:
    void exitApp();                                             // exits the App
    void inputMessage();                                        // handles input from QLineEdit
    void displayMessage(QString);                               // displays messages from server

signals:
    void started();                                             // signal to start listening
private:
    Ui::chatClient *ui;
    QInputDialog connectDialog;                                 // for username input
    QMessageBox serverErrBox;                                   // for when server isnt running
    QMessageBox nameErr;                                        // for when username is already used
    msgSendRecv *chat;                                          // handles sending and receiving
    QString usrname;                                            // holds the username
    int usrid;                                                  // holds an arbitrary id
    QThread *thread;                                            // thread

protected:
    void connectAs();                                           // username input function
    void createClient();                                        // creates a chat client and mailbox
    void sendMsg(QString message);                              // sends a msg to the server
};

#endif // CHATCLIENT_H
