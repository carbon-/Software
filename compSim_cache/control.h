/*
 * control.h
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 5
 * parekhsm@vt.edu
 * 10/19/2012
 */
#ifndef CONTROL_H
#define CONTROL_H
#include <QStringList>
#include <QPair>
#include <QRegExp>
typedef int16_t word;

#define MASK 0xFF;

enum errors
{
    instruction,
    stop,
    instmem,
    datamem
};

class control : public QObject
{
    Q_OBJECT

public:
    explicit control(QObject *parent = 0);
    ~control();


    void addNumbers(word value);            // adds two numbers together
    word twosComplement(word toTwo);        // converts to and from two's complement
    void updatePC();                        // increments the PC register

private:
    word currentPC;                         // stores the PC register
    word currentAC;                         // stores the Accumulator value
    QString currentInst;                    // stores the current instruction
    bool stepMode;                          // checks if operate mode is step
    void exeInstruction(QString inst);      // executes the inst instruction


public slots:
    void resetControl();                    // resets the control module
    void getcurInst(QString curI);          // gets the current instruction from instruction memory
    void getcurData(word curData);          // gets the data from data memory at address
    void stepPgm(int i);                    // checks the step check box for stepping
    void runSim();                          // runs the simulator
    void resetPC();

signals:
    void setData(word addr, word value);    // sets the value at address in the data memory
    void getInst(word);                     // gets the instruction from the instruction memory
    void getData(word addr);                // gets the data from address
    void getCData(word addr);
    void setCData(word value, word addr);
    void stopComputer(int);                 // stops the computer if in case of error or HALT instruction
    void changePCInst(QString);             // updates the GUI
    void changePCCount(int);                // updates the GUI
    void showAC(QString);                   // updates the GUI
    void flushCache();                      // write all dirty blocks back to mem
};

#endif // CONTROL_H
