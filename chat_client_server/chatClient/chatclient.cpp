/*
 * chatclient.cpp
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 6
 * parekhsm@vt.edu
 * 11/15/2012
 *
 */

#include "chatclient.h"
#include "ui_chatclient.h"

chatClient::chatClient(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatClient)
{
    ui->setupUi(this);
    usrid = id;
    // connect gui signals to slots
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(exitApp()));
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(inputMessage()));

    // input username and create a chat client
    connectAs();
    createClient();

    // connect message send and receive signals/slots
    connect(chat, SIGNAL(messageRecv(QString)), this, SLOT(displayMessage(QString)));
    
    connect(this, SIGNAL(started()), chat, SLOT(startRecv()));

    // start reading mailbox for messages
    emit started();

}

chatClient::~chatClient()
{
    delete ui;
}

// Exits the chat client
void chatClient::exitApp()
{
    // update server user list and exit
    chat->sendMsg(0, "Remove");
    chat->~msgSendRecv();
    this->close();
}

// Displays the message from server to TextEdit
void chatClient::displayMessage(QString msg)
{
    // display message from server
    QStringList fmtMsg;
    fmtMsg = msg.split(QRegExp("\\|"), QString::SkipEmptyParts);
    ui->plainTextEdit->appendPlainText(fmtMsg.at(1));
}

// connects to the server with a specified username
void chatClient::connectAs()
{
    // input a user name, sets up dialog box
    bool ok;
    QString name, title;
    nameErr.setWindowTitle("Error");
    while (name.isEmpty())
    {
        name = connectDialog.getText(this, tr("Connect as"), tr("User name:"), QLineEdit::Normal, QDir::home().dirName(), &ok);
        //nameErr.setText("User name already in use, please input another user name");
        //nameErr.exec();
        //name.clear();
    }
    title = name + "'s chat window";
    this->setWindowTitle(title);
    usrname = name;

    // DEBUG
    //qDebug() << usrid;
    //qDebug() << usrname;

}

// creates a chat client
void chatClient::createClient()
{
    QString mail = "/mailbox";
    mail.append(QString("%1").arg(usrid));
    std::string mid = mail.toLatin1().constData();

    thread = new QThread();
    chat = new msgSendRecv(usrname, mid.c_str());
    chat->moveToThread(thread);
    thread->start();
    chat->sendMsg(0, "Add");
}

// sends a message to the server
void chatClient::sendMsg(QString message)
{
    chat->sendMsg(0, message);
}

// grabs the message from LineEdit
void chatClient::inputMessage()
{
    QString messg = ui->lineEdit->text();
    // DEBUG
    //qDebug() << messg;

    ui->lineEdit->clear();
    sendMsg(messg);
}
