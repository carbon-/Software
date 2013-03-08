/*
 * compgui.h
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 5
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
#include "cache.h"

namespace Ui {
class compGui;
}

class compGui : public QMainWindow
{
    Q_OBJECT
    
protected slots:
    virtual void stopComp(int errorType);
    virtual void openFile();
    virtual void exitApp();
    virtual void updateMemTable(int, QString);
    virtual void updateInstTable(int, QString);
    virtual void resetComp();
    virtual void clearComp();
    virtual void cacheRows(int);
    virtual void cacheBlocks(int);
    virtual void updateCacheTable(int, int, int valid, int, int);
    virtual void updateColor(int);

signals:
    void setInst(QString, int);
    void reset();
    void resetPC();
    void resetData();
    void disable(bool);
    void disableCache(bool);

public:
    explicit compGui(QWidget *parent = 0);
    ~compGui();
    QTableView *cacheView;
    void createComboBox();
    void createCacheTable();
    void clearCacheGUI();
    
private:
    Ui::compGui *ui;
    memory myMemory;
    control myControl;
    cache myCache;
    QString instructions;
    bool stopped;
    QStandardItemModel *tableModel;
    QStandardItemModel *cacheModel;
    int cRows;
    int cBlocks;

protected:
    // Loads the file for the program to use
    virtual void loadFile(const QString &filename);
    // Sets the current file as window title
    virtual void setCurrentFile(const QString &fileName);
    // returns the name stripped of the path
    QString strippedName(const QString &fullFileName);

    // holds labels for table
    QStringList memLabels;

    QStringList cacheLabels;

    // Generates the hexadecimal address for table
    void generateLabels();

    void generateCacheLabels();

    // creates a status bar
    virtual void createStatusBar();

    // holds the file name
    QString myFile;

    // holds the label
    QLabel* statusLabel;
};

#endif // COMPGUI_H
