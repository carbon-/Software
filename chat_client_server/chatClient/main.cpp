/*
 * main.cpp
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 6
 * parekhsm@vt.edu
 * 11/15/2012
 *
 */
#include <QApplication>
#include <iostream>
#include "chatclient.h"
#include "chatter.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (argc != 2)
    {
        qDebug() << "Invalid Usage!\nUsage: ./chatClient <number of clients>";
        return 0;
    }

    // create an instance of a thread and chat client
    chatter *chatC;
    QThread *thread;

    // grab how many chat clients to create
    int clients = (int)argv[1][0]-0x30;

    // create specified number of chat clients
    for (int i = 0; i < clients; i++)
    {
        thread = new QThread();
        chatC = new chatter(i+1);
        // move each client to a new thread
        chatC->moveToThread(thread);
        thread->start();
        // DEBUG
        //qDebug() << chatC->thread();
    }

    return a.exec();
}
