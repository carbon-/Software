/*
 * memory.cpp
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 4
 * parekhsm@vt.edu
 * 10/19/2012
 */
#include "memory.h"

// Constructor for memory module
// memory module contains the instruction and data memory
memory::memory(QObject *parent)
{
    resetMem();
}

// Destructor for memory module
memory::~memory()
{}

// Resets the instructions and data memories
void memory::resetMem()
{
    for (int i = 0; i < MAX_DATA_MEM; i++)
    {
        dataMem[i] = 0;

    }
    for (int i = 0; i < MAX_INST_MEM; i++)
    {
        instMem[i] = "";
    }
}

// SLOT function to set data at specified address
// Also emits a signal to update the memory table
// on the GUI
void memory::setDataAtAddr(word addr, word data)
{
    dataMem[addr] = data;
    QString dataStr = toHexString(data);
    emit updateTable(addr, dataStr);
}

// SLOT function to get data at specified address
void memory::getDataAtAddr(word addr)
{
    emit returnData(dataMem[addr]);
}

// SLOT function to get instruction based on the
// program counter
void memory::getInst(word pos)
{
    emit returnInst(instMem[pos]);
}

// SLOT function to set instruction based on a
// position, useful when reading instructions from
// file
void memory::setInstMem(QString newInst, int i)
{
    instMem[i] = newInst;
}

// Turns the number stored in data memory to a two's complement
// hexadecimal string
QString memory::toHexString(word numb)
{
    numb ^= 0xFF;
    numb += 1;
    QString Hex = QString::number(numb, 16);
    if (numb < 0)
        Hex.remove(0, 14);
    return Hex;
}
