/*
 * main.cpp
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 3
 * parekhsm@vt.edu
 * 09/29/2012
 */
#include <QApplication>
#include <QTextEdit>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
