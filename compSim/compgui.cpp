/*
 * compgui.cpp
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 4
 * parekhsm@vt.edu
 * 10/19/2012
 */
#include "compgui.h"
#include "ui_compgui.h"

// Constructor for the computer GUI program.
// Connects signals and slots from the control module, memory module
// also sets up the table
compGui::compGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::compGui)
{

    ui->setupUi(this);
    // Set up a table to display the memory contents
    tableModel = new QStandardItemModel(512,0, this);
    tableModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Memory")));
    // generate the labels in hexadecimal
    generateLabels();
    // set the labels
    tableModel->setVerticalHeaderLabels(memLabels);
    // set the model to gui
    ui->memTable->setModel(tableModel);

    // signal/slot for opening files
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));

    // signal/slot for exiting the application
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitApp()));

    // signal/slot for stepping through the program
    connect(ui->checkStep, SIGNAL(stateChanged(int)), &myControl, SLOT(stepPgm(int)));

    // signal/slot for run/step through the program
    connect(ui->run_button, SIGNAL(clicked()), &myControl, SLOT(runSim()));

    // signal/slot for the OUT Instruction
    connect(&myControl, SIGNAL(showAC(QString)), ui->console, SLOT(append(QString)));

    // signal/slot for displaying the current Instruction
    connect(&myControl, SIGNAL(changePCInst(QString)), ui->PCInst, SLOT(setText(QString)));

    // signal/slot for displaying the current PC
    connect(&myControl, SIGNAL(changePCCount(int)), ui->PCCount, SLOT(setNum(int)));

    // signal/slot for setting instructions from file into the instruction memory
    connect(this, SIGNAL(setInst(QString, int)), &myMemory, SLOT(setInstMem(QString, int)));

    // signal/slot for resetting the control and memory module
    connect(this, SIGNAL(reset()), &myControl, SLOT(resetControl()));
    connect(this, SIGNAL(reset()), &myMemory, SLOT(resetMem()));

    // signal/slot for updating the table if a value is changed in the memory
    connect(&myMemory, SIGNAL(updateTable(int,QString)), this, SLOT(updateMemTable(int,QString)));

    // signal/slot for stopping the simulator if an error occurs, or halt instruction is reached
    connect (&myControl, SIGNAL(stopComputer(bool)), this, SLOT(stopComp(bool)));

    // signal/slot to request the instruction based on the PC from the instruction memory
    connect(&myControl, SIGNAL(getInst(word)), &myMemory, SLOT(getInst(word)));

    // signal/slot for returning the instruction from instruction memory to control module
    connect(&myMemory, SIGNAL(returnInst(QString)), &myControl, SLOT(getcurInst(QString)));

    // signal/slot to request data at an address from data memory
    connect(&myControl, SIGNAL(getData(word)), &myMemory, SLOT(getDataAtAddr(word)));

    // signal/slot for returning data at an address from data memory to control module
    connect(&myMemory, SIGNAL(returnData(word)), &myControl, SLOT(getcurData(word)));

    // signal/slot for setting the data at an adress in the data memory, control to memory
    connect(&myControl, SIGNAL(setData(word,word)), &myMemory, SLOT(setDataAtAddr(word,word)));

}

// Destructor for GUI
compGui::~compGui()
{
    delete ui;
}

// Handles opening program files
void compGui::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        loadFile(fileName);
    }
}

// Handles loading of files, also resets the control module, memory module, table and GUI
void compGui::loadFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Comp Sim"), tr("Cannot read file %1:\n%2").arg(filename).arg(file.errorString()));
        return;
    }

    // Read and process the file
    QTextStream in(&file);
    instructions = in.readAll();

    // reset control and memory module
    emit reset();

    // reset table and GUI
    generateLabels();
    ui->PCCount->clear();
    ui->PCInst->clear();
    stopped = false;
    ui->console->clear();

    // Put all the instructions in a list
    QStringList iList = instructions.split(QRegExp("\\n"), QString::SkipEmptyParts);

    // if instruction list > instruction memory throw error, stop simulator
    if (iList.size() > 128)
    {
        ui->console->append(QString("Error: Number of Instructions greater than 128."));
        stopComp(true);
    }
    // else put them into the instruction memory
    else
    {
        for (int i = 0; i < iList.size(); i++)
        {
            emit setInst(iList.at(i), i);
        }
    }

    // set the current file as the window title
    setCurrentFile(filename);
    // Show success status
    statusBar()->showMessage(tr("File loaded. Simulator ready."), 10000);

 }

// Stops the simulator from running if there is an error
// or if HALT was reached
void compGui::stopComp(bool error)
{
    if (!stopped && error)
    {
        ui->console->append("Error: Memory/Instruction Access beyond range!");
        stopped = true;
    }
    else if (!stopped)
    {
        ui->console->append("HALT Instruction Reached, simulator stopped");
        stopped = true;
    }
}

// Exits the application
void compGui::exitApp()
{
    qApp->exit();
}

// Creates a status bar at the bottom of the window
void compGui::createStatusBar()
{
    statusLabel = new QLabel(tr("Status Label"));
    statusLabel->setAlignment(Qt::AlignHCenter);
    statusLabel->setMinimumSize(statusLabel->sizeHint());
    statusLabel->clear();

    statusBar()->addWidget(statusLabel,0);
}

// Sets the current file as the window title
void compGui::setCurrentFile(const QString &fileName)
{
    myFile = fileName;
    if (myFile.isEmpty())
    {
        setWindowTitle(tr("Computer GUI"));
    }
    else
    {
        setWindowTitle(tr("%1 - %2").arg(strippedName(myFile)).arg(tr("Computer GUI")));
    }
}

// Returns the filename stripped of its path
QString compGui::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

// Generates the labels for memory as hexadecimal and initializes all of the cells as 0
void compGui::generateLabels()
{
    memLabels.clear();
    for (int i = 0; i < 512; i++)
    {
        QStandardItem *memValue = new QStandardItem(QString("0"));
        memLabels.append(QString::number(i, 16));
        tableModel->setItem(i, memValue);
    }
}

// SLOT function to update the GUI if there is a change in data memory
void compGui::updateMemTable(int pos, QString dataStr)
{
    QStandardItem *memValue = new QStandardItem(dataStr);
    tableModel->setItem(pos, memValue);
}
