/*
 * control.cpp
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 4
 * parekhsm@vt.edu
 * 10/19/2012
 */
#include "control.h"
#include <QDebug>

// Constructor for control module
control::control(QObject *parent)
{
    resetControl();
}

// Destructor for control module
control::~control()
{}

// Resets values for the Accumulator, Program Counter and
// Current Instruction
void control::resetControl()
{
    currentPC = 0;
    currentAC = 0;
    currentInst = "NOP";
    stepMode = false;
}

// Runs the simulation, if step mode is enabled
// it runs an instruction at a time
void control::runSim()
{
    // if step mode is disabled, run until halt is
    // encountered
    if (!stepMode)
        while (currentInst != "HALT")
        {
            // get the instruction from instruction memory
            emit getInst(currentPC);
            // update the pc instruction and count on GUI
            emit changePCInst(currentInst);
            emit changePCCount(currentPC);
            // execute the instruction
            exeInstruction(currentInst);
        }
    else
    {
        // get the instruction from instruction memory
        emit getInst(currentPC);
        // update the pc instruction and count on GUI
        emit changePCInst(currentInst);
        emit changePCCount(currentPC);
        // execute the instruction
        exeInstruction(currentInst);
    }

}

void control::exeInstruction(QString inst)
{
    // some temporary variables
    QStringList temp;
    word value;
    bool ok;
    // split the instruction by whitespace
    // for instructions that require two
    // arguments
    temp = inst.split(QRegExp("\\s+"));

    // LOAD Instruction
    // LOAD <hex-data-address>
    // Loads a value from the memory address <hex-data-address>
    // into the accumulator. Valid range from 0 to 511.
    if (temp.at(0) == "LOAD")
    {
        // get address
        value = temp.at(1).toInt(&ok, 16);
        // error check
        if (value < 0 || value > 511)
            stopComputer(true);
        // send signal to get data from data memory at address
        // specified by value
        else
            emit getData(value);
    }

    // STORE Instruction
    // STORE <hex-data-address>
    // Stores the value in the accumulator to memory address
    // <hex-data-address>. Valid range from 0 to 511.
    else if (temp.at(0) == "STORE")
    {
        // get address
        value = temp.at(1).toInt(&ok, 16);

        // error check
        if (value < 0 || value > 511)
            emit stopComputer(true);
        // send signal to set data
        // address is specified by value
        // store two's complement of the data in memory.
        else
        {
            word data = twosComplement(currentAC);
            emit setData(value, data);
        }
    }

    // ADDV Instruction
    // ADDV <signed-decimal-number>
    // Adds the signed decimal number to the accumulator.
    else if (temp.at(0) == "ADDV")
    {
        // get the signed number from Instruction
        value = temp.at(1).toInt();
        // add the value to the accumulator
        addNumbers(value);
    }

    // ADD Instruction
    // ADD <hex-data-address>
    // Adds the data at hex address <hex-data-address> to the value in
    // the accumulator.
    else if (temp.at(0) == "ADD")
    {
        // get address from instruction
        value = temp.at(1).toInt();
        // convert accumulator to two's complement
        word tempVal = twosComplement(currentAC);
        // get data from address specified by value
        emit getData(value);
        // add the numbers together
        addNumbers(tempVal);
    }

    // BGTZ Instruction
    // BGTZ <decimal-instruction-address>
    // Branches if the value in accumulator is greater than zero.
    // If the value is less than or equal to zero, PC is incremented by 1.
    else if (temp.at(0) == "BGTZ")
    {
        // get decimal address from instruction
        value = temp.at(1).toInt();
        // if accumulator is greater than zero, branch
        if (currentAC > 0)
        {
            // error check
            if (value < 0 || value > 127)
                stopComputer(true);
            // set the PC to branch address
            else
                currentPC = value;
        }
    }

    // CLEAR Instruction
    // CLEAR
    // Sets the accumulator to zero.
    else if (temp.at(0) == "CLEAR")
    {
        currentAC = 0;
    }

    // HALT Instruction
    // HALT
    // Halts the simulator. Used as last instruction of a program
    // PC does not increase after this is called.
    else if (temp.at(0) == "HALT")
    {
        emit stopComputer(false);
    }

    // OUT Instruction
    // OUT <string-with-possible-accumulator-value>
    // Outputs the current value of the accumulator in a custom string
    // specified by the instruction.
    else if (temp.at(0) == "OUT")
    {
        // do regular expression to extract custom string from instruction
        QStringList out;
        out = inst.split(QRegExp("\""), QString::SkipEmptyParts);
        // replace the %A value with the accumulator
        QString outStr = out.at(1);
        outStr.replace("%A", QString::number(currentAC, 10));
        // display it on the GUI console.
        emit showAC(outStr);
    }
    // NOP Instruction
    // NOP
    // Increments the PC
    else if (temp.at(0) == "NOP")
    {
        // No Operation does nothing
    }

    // only update the PC if instruction is not halt and bgtz
    if (temp.at(0) != "HALT" && temp.at(0) != "BGTZ")
        updatePC();
}

// SLOT Function to get the current Instruction from
// the instruction memory
void control::getcurInst(QString curI)
{
    currentInst = curI;
}

// SLOT Function to get data from an address from
// the data memory
void control::getcurData(word curData)
{
    currentAC = twosComplement(curData);
}

// Checks the state of the step check box
// if the box is checked, the program will "step"
// else it will run
void control::stepPgm(int i)
{
    if (i == 2)
        stepMode = true;
    else if (i == 0)
        stepMode = false;
}

// Adds two numbers together
void control::addNumbers(word value)
{
    currentAC += value;
}

// converts a number from and to two's complement
word control::twosComplement(word toTwo)
{
    toTwo ^= MASK;
    toTwo += 1;
    return toTwo;
}

// Increments the Program Counter by one
void control::updatePC()
{
    currentPC += 1;
}
