/*
 * main.cpp
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 4
 * parekhsm@vt.edu
 * 10/19/2012
 */
#include <QApplication>
#include "compgui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    compGui w;
    w.show();
    
    return a.exec();
}
