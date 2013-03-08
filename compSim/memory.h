/*
 * memory.h
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 4
 * parekhsm@vt.edu
 * 10/19/2012
 */
#ifndef MEMORY_H
#define MEMORY_H
#include <QObject>
#include <QDebug>
#include <QList>

typedef int16_t word;
#define MAX_DATA_MEM 512
#define MAX_INST_MEM 128



class memory : public QObject
{
    Q_OBJECT
public:
    explicit memory(QObject *parent = 0);
    ~memory();
    QString toHexString(word numb);             // converts word to Hexadecimal String


private:
    word dataMem[MAX_DATA_MEM];                 // data memory
    QString instMem[MAX_INST_MEM];              // instruction memory

protected slots:
    void resetMem();                            // resets the memory module
    void getInst(word);                         // returns the instruction at position
    void getDataAtAddr(word addr);              // returns the data at address
    void setInstMem(QString newInst, int i);    // sets the instruction at position i
    void setDataAtAddr(word addr, word data);   // sets data at address

signals:
    void returnInst(QString curI);              // returns the instruction requested
    void returnData(word data);                 // returns the data requested
    void updateTable(int pos, QString data);    // updates the GUI
};

#endif // MEMORY_H
