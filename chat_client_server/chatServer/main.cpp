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
#include "chatserver.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    chatServer w;
    w.show();
    
    return a.exec();
}
