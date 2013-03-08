/*
 * compgui.h
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 4
 * parekhsm@vt.edu
 * 10/19/2012
 */
#ifndef COMPGUI_H
#define COMPGUI_H

#include <QMainWindow>
#include <QLabel>
#include <QtGui>

#include "control.h"
#include "memory.h"

namespace Ui {
class compGui;
}

class compGui : public QMainWindow
{
    Q_OBJECT
    
protected slots:
    virtual void stopComp(bool error);
    virtual void openFile();
    virtual void exitApp();
    virtual void updateMemTable(int, QString);
    //virtual void debug(bool toggled);

signals:
    void setInst(QString, int);
    void reset();

public:
    explicit compGui(QWidget *parent = 0);
    ~compGui();
    
private:
    Ui::compGui *ui;
    memory myMemory;
    control myControl;
    QString instructions;
    bool stopped;
    QStandardItemModel *tableModel;

protected:
    // Loads the file for the program to use
    virtual void loadFile(const QString &filename);
    // Sets the current file as window title
    virtual void setCurrentFile(const QString &fileName);
    // returns the name stripped of the path
    QString strippedName(const QString &fullFileName);

    // holds labels for table
    QStringList memLabels;

    // Generates the hexadecimal address for table
    void generateLabels();

    // creates a status bar
    virtual void createStatusBar();

    // holds the file name
    QString myFile;

    // holds the label
    QLabel* statusLabel;
};

#endif // COMPGUI_H
