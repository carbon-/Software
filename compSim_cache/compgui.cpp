/*
 * compgui.cpp
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 5
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
    tableModel = new QStandardItemModel(512,1, this);
    tableModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Memory")));
    tableModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Instruction")));


    // generate the labels in hexadecimal
    generateLabels();
    // set the labels
    tableModel->setVerticalHeaderLabels(memLabels);
    // set the model to gui
    ui->memTable->setModel(tableModel);

    createComboBox();
    cRows = 8;
    cBlocks = 2;

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

    // signal/slot for resetting the control, cache and memory module
    connect(this, SIGNAL(reset()), &myControl, SLOT(resetControl()));
    connect(this, SIGNAL(reset()), &myMemory, SLOT(resetMem()));
    connect(this, SIGNAL(reset()), &myCache, SLOT(clearCache()));

    // signal/slot for updating the memory table if a value is changed in the memory
    connect(&myMemory, SIGNAL(updateTable(int,QString)), this, SLOT(updateMemTable(int,QString)));

    // signal/slot for updating the instruction table
    connect(&myMemory, SIGNAL(updateITable(int, QString)), this, SLOT(updateInstTable(int, QString)));

    // signal/slot for stopping the simulator if an error occurs, or halt instruction is reached
    connect (&myControl, SIGNAL(stopComputer(int)), this, SLOT(stopComp(int)));

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

    // signal/slot for disabling run button if an error occurs
    connect(this, SIGNAL(disable(bool)), ui->run_button, SLOT(setDisabled(bool)));

    // signal/slot for disabling step button
    connect(this, SIGNAL(disable(bool)), ui->checkStep, SLOT(setDisabled(bool)));

    // signal/slot for disabling cache parameters during run time
    connect(this, SIGNAL(disableCache(bool)), ui->cacheSize, SLOT(setDisabled(bool)));
    connect(this, SIGNAL(disableCache(bool)), ui->blockSize, SLOT(setDisabled(bool)));


    // signal/slot for resetting the computer
    connect(ui->reset, SIGNAL(clicked()), this, SLOT(resetComp()));

    // signal/slot for clearing the computer
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clearComp()));

    // signal/slot for resetting the Program Counter and Accumulator
    connect(this, SIGNAL(resetPC()), &myControl, SLOT(resetPC()));

    // signal/slot for resetting the data memory
    connect(this, SIGNAL(resetData()), &myMemory, SLOT(resetData()));

    // signal/slot for changing cache size
    connect(ui->cacheSize, SIGNAL(currentIndexChanged(int)), &myCache, SLOT(changeCacheSize(int)));

    // signal/slot for changing block size
    connect(ui->blockSize, SIGNAL(currentIndexChanged(int)), &myCache, SLOT(changeBlockSize(int)));

    // signal/slot to change cache GUI
    connect(&myCache, SIGNAL(returnBlocks(int)), this, SLOT(cacheBlocks(int)));
    connect(&myCache, SIGNAL(returnRows(int)), this, SLOT(cacheRows(int)));

    // signal/slot to update hits/ratio on GUI
    connect(&myCache, SIGNAL(updateRatio(double)), ui->ratio, SLOT(display(double)));
    connect(&myCache, SIGNAL(updateHits(int)), ui->hits, SLOT(display(int)));

    // signal/slot to read value into cache from memory
    connect(&myCache, SIGNAL(readBlockFromMem(word)), &myMemory, SLOT(getCDataAtAddr(word)));

    // signal/slot to write value from cache to memory
    connect(&myCache, SIGNAL(writeBlockToMem(word,word)), &myMemory, SLOT(setCDataAtAddr(word,word)));

    // signal/slot to return data to cache from memory
    connect(&myMemory, SIGNAL(returnCData(word)), &myCache, SLOT(setData(word)));

    // signal/slot to set accumulator to data from cache
    connect(&myCache, SIGNAL(setAccumulator(word)), &myControl, SLOT(getcurData(word)));

    // signal/slot to get data from cache to the control
    connect(&myControl, SIGNAL(getCData(word)), &myCache, SLOT(readFromCache(word)));

    // signal/slot to write data to cache from the control
    connect(&myControl, SIGNAL(setCData(word,word)), &myCache, SLOT(writeToCache(word,word)));

    // singal/slot to update the cache GUI
    connect(&myCache, SIGNAL(updateGUI(int,int,int,int,int)), this, SLOT(updateCacheTable(int,int,int,int,int)));

    // signal/slot to color table cell in cache GUI
    connect(&myCache, SIGNAL(setTableColor(int)), this, SLOT(updateColor(int)));

    // signal/slot to flush cache
    connect(&myControl, SIGNAL(flushCache()), &myCache, SLOT(flushCache()));

    disable(true);


}

void compGui::createCacheTable()
{

    cacheView = new QTableView;
    cacheView->setWindowTitle("Cache");
    cacheModel = new QStandardItemModel(cRows,cBlocks-1, this);
    for (int i = 0; i < cBlocks; i++)
    {
        cacheModel->setHorizontalHeaderItem(i, new QStandardItem(QString("Block %1").arg(i)));
    }
    cacheView->setEditTriggers(QTableView::NoEditTriggers);

    generateCacheLabels();
    cacheModel->setVerticalHeaderLabels(cacheLabels);
    clearCacheGUI();
    cacheView->setModel(cacheModel);

    cacheView->resizeColumnsToContents();
    cacheView->resizeRowsToContents();
}

void compGui::clearCacheGUI()
{
    for (int i = 0; i < cRows; i++)
    {
        for (int j = 0; j < cBlocks; j++)
        {
            cacheModel->setItem(i,j, new QStandardItem(QString("Tag: 0\nValid: 0\nData: 0")));
        }
    }
}

void compGui::createComboBox()
{
    ui->cacheSize->addItem("16 words", 16);
    ui->cacheSize->addItem("32 words", 32);
    ui->cacheSize->addItem("64 words", 64);

    ui->blockSize->addItem("2 words", 2);
    ui->blockSize->addItem("4 words", 4);
    ui->blockSize->addItem("8 words", 8);
}

// Destructor for GUI
compGui::~compGui()
{
    cacheView->close();
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

void compGui::resetComp()
{
    ui->PCCount->clear();
    ui->PCInst->clear();
    stopped = false;
    ui->console->clear();
    ui->console->append(QString("Computer Reset: PC and Accumulator set to zero, Data Memory cleared."));
    disable(false);
    clearCacheGUI();
    emit resetPC();
    emit resetData();
}

void compGui::clearComp()
{
    cacheView->close();
    emit reset();
    clearCacheGUI();
    ui->PCCount->clear();
    ui->PCInst->clear();
    stopped = false;
    ui->console->clear();
    ui->console->append(QString("Computer Reset: PC and Accumulator set to zero, Data & Instruction Memory cleared."));
    disable(true);
    disableCache(false);
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

//    // reset control and memory module
//    emit reset();

//    // reset table and GUI
//    generateLabels();
//    ui->PCCount->clear();
//    ui->PCInst->clear();
//    stopped = false;
//    ui->console->clear();

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

    disable(false);
    disableCache(true);
    createCacheTable();
    cacheView->show();
 }

// Stops the simulator from running if there is an error
// or if HALT was reached
void compGui::stopComp(int errorType)
{
    // Invalid instruction memory access
    if (errorType == instmem)
    {
        ui->console->append("Error: Instruction Memory Access beyond range!");
        ui->console->append("Check program file and try again.");
        stopped = true;
    }

    // Invalid data memory access
    else if (errorType == datamem)
    {
        ui->console->append("Error: Data Memory Access beyond range!");
        ui->console->append("Check program file and try again.");
        stopped = true;
    }

    // Invalid instruction encountered
    else if (errorType == instruction)
    {
        ui->console->append("Error: Invalid Instruction!");
        ui->console->append("Check program file and try again.");
        stopped = true;
    }

    // HALT instruction reached
    else if (errorType == stop)
    {
        ui->console->append("HALT Instruction Reached, simulator stopped");
        stopped = true;
    }

    // Shouldnt happen as all error types should be enumerated.
    else
    {
        ui->console->append("Some error occured!");
        ui->console->append("Check program file and try again.");
        stopped = true;
    }
    // if any error is encountered run/step button is disabled
    if (stopped)
    {
        disable(true);
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
        tableModel->setItem(i, 0, memValue);

    }
    for (int i = 0; i < 128; i++)
    {
        QStandardItem *instValue = new QStandardItem(QString("NOP"));
        tableModel->setItem(i, 1, instValue);
    }

}

void compGui::generateCacheLabels()
{
    cacheLabels.clear();
    for (int i = 0; i < cRows; i++)
    {
        cacheLabels.append(QString::number(i, 16));

    }
}

// SLOT function to update the GUI if there is a change in data memory
void compGui::updateMemTable(int pos, QString dataStr)
{
    QStandardItem *memValue = new QStandardItem(dataStr);
    tableModel->setItem(pos, 0, memValue);
}

// SLOT function to update the GUI's Instruction table
void compGui::updateInstTable(int pos, QString dataStr)
{
    QStandardItem *instValue = new QStandardItem(dataStr);
    tableModel->setItem(pos, 1, instValue);
}

// SLOT function to update the GUI if there is a change in cache memory
void compGui::updateCacheTable(int value, int tag, int valid, int row, int column)
{
    cacheModel->setItem(row,column, new QStandardItem(QString("Tag: %1\nValid: %3\nData: %2").arg(tag).arg(value).arg(valid)));
    cacheView->resizeColumnsToContents();
    cacheView->resizeRowsToContents();
}

void compGui::updateColor(int color)
{
    //ui->console->append(QString("%1 %2").arg(row).arg(column));
    QBrush brush;
//    if (color == white)
//    {
//        brush.setColor(Qt::white);
//        brush.setStyle(Qt::SolidPattern);
//        for (int i = 0; i < cRows; i++)
//        {
//            for (int j = 0; j < cBlocks; j++)
//            {
//                cacheModel->item(i,j)->setBackground(brush);
//            }
//        }
//    }
    QString posStr = ui->PCCount->text();
    int pos = posStr.toInt();
    if (color == red)
    {


        brush.setColor(Qt::red);
        brush.setStyle(Qt::SolidPattern);
        tableModel->item(pos,1)->setBackground(brush);
    }
    else if (color == green)
    {
        brush.setColor(Qt::darkGreen);
        brush.setStyle(Qt::SolidPattern);
        tableModel->item(pos, 1)->setBackground(brush);
    }

    ui->memTable->setModel(tableModel);
}

void compGui::cacheBlocks(int b)
{
    //cacheView->close();
    cBlocks = b;
    ui->console->append(QString("The block size is now %1").arg(cBlocks));
    createCacheTable();
}

void compGui::cacheRows(int r)
{
    //cacheView->close();
    cRows = r;
    ui->console->append(QString("The row size is now %1").arg(cRows));
    createCacheTable();
}
