/*
 * mainwindow.cpp
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 3
 * parekhsm@vt.edu
 * 09/29/2012
 */
#include <QtGui>
#include "mainwindow.h"
#include "Dictionary.h"
#include "ui_mainwindow.h"
#include <iostream>

using namespace std;

// function to sort the most frequent words in descending order
bool sortDescending(item& aword, item &bword)
{
    if(aword.second < bword.second)
        return false;
    else
        return true;
}

// Constructor for main window class
// creates the combo box, status bar, subwindows
// connects the signals from the main window, combo box, spin box to their slots
// sets the default language
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    createComboBox();
    createStatusBar();
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(exitApp()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLang(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(amountToDisplay(int)));
    setWindowTitle(tr("Dictionary GUI"));

    createSubWindows();
    language = "/usr/share/dict/american-english";
}

// destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// Opens a file dialog to open the user selected file
// then calls the loadFile function to load the user selected file
// called when action_Open is triggered
void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        loadFile(fileName);
    }
}

// Exits the application
// called when action_Exit is triggered
// also closes the sub windows

void MainWindow::exitApp()
{
    wc->close();
    sc->close();
    qApp->exit();
}

// creates a status bar at the bottom of the main window with the
// label widget added
void MainWindow::createStatusBar()
{
    statusLabel = new QLabel(tr("Status Label"));
    statusLabel->setAlignment(Qt::AlignHCenter);
    statusLabel->setMinimumSize(statusLabel->sizeHint());
    statusLabel->clear();

    statusBar()->addWidget(statusLabel,0);
}

// creates and sets attributes to the two text edit windows word count
// and spell check
void MainWindow::createSubWindows()
{
    wc = new QTextEdit;
    sc = new QTextEdit;
    wc->setWindowTitle("Word Count");
    sc->setWindowTitle("Spell Check");
    wc->setMinimumSize(500,300);
    wc->setAcceptRichText(true);
    sc->setMinimumSize(500,300);
    sc->setAcceptRichText(true);
}

// creates a combo box for language selection
void MainWindow::createComboBox()
{
    ui->comboBox->addItem("American-English", 0);
    ui->comboBox->addItem("British-English", 1);
}

// Loads file and displays it to the main window text edit
// also sets the main window title to file name
// creates the dictionary and outputs results for spellCheck
// and wordCount
void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Dictionary GUI"), tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
    text.clear();
    // read the file
    QTextStream in(&file);
    text = in.readAll();
    ui->textEdit->setPlainText(text);
    QApplication::restoreOverrideCursor();
    // set file name as window title
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded."), 2000);
    // call the dictionary class to do spell check and word count
    createDictionary();
}

// Creates a dictionary based on the language and spell checks the text
// also counts the most frequent correct words
void MainWindow::createDictionary()
{
    // create the dictionary
    Dictionary myDict(language);
    // remove all punctuation
    QStringList inWords = text.split(QRegExp("\\W+"), QString::SkipEmptyParts);

    // run the stripped text through the dictionary
    myDict.isMisspelled(inWords);

    mW.clear();
    cW.clear();

    // get back a map of the missed words and correct words
    mW = myDict.getMissedWords();
    cW = myDict.getWordCount();

    // call the spellCheck and wordCount functions to format and output results
    spellCheck();
    wordCount();
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    // sets the file name as window title if a file is loaded
    // else names the main window Dictionary GUI
    myFile = fileName;
    statusLabel->clear();
    if(myFile.isEmpty())
    {
        setWindowTitle(tr("Dictionary GUI"));
    }
    else
    {
        setWindowTitle(tr("%1 - %2").arg(strippedName(myFile)).arg(tr("Dictionary GUI")));
    }
}

// strips the path out of the file name
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

// slot function to change the language
// changes the language based on the index position of the combo box
// the default language is American-English at index 0, British-English is at
// index 1
void MainWindow::changeLang(int index)
{
    if (index == 0)
        language = "/usr/share/dict/american-english";
    else if (index == 1)
        language = "/usr/share/dict/british-english";
    createDictionary();
}

// slot function to change the amount of correct words to display
// calls displayWords after setting the displayAmt variable
void MainWindow::amountToDisplay(int amount)
{
    displayAmt = amount;

    displayWords();
}

// formats the missed words in red and outputs them to the
// spell check window
void MainWindow::spellCheck()
{
    sc->clear();
    QString sred = "<font color ='red'>";
    QString ered = "</font>";
    QString temp;
    QList<QString> missedlist = mW.keys();
    QString fmtText = text;
    for (int i = 0; i < missedlist.size(); i++)
    {
        temp = sred+missedlist.at(i)+ered;
        fmtText.replace(missedlist.at(i), temp);
    }
    if (fmtText.contains(QString("\n")))
    {
        fmtText.replace(QString("\n"), QString("<br>"));
    }
    sc->setHtml(fmtText);
    sc->show();
}

// sorts the correct words in descending order and calls
// displayWords() to display them to the word count window
void MainWindow::wordCount()
{
    cV.clear();
    QMapIterator<QString, int> i(cW);
    while(i.hasNext())
    {
        i.next();
        item temppair;
        temppair.first = i.key();
        temppair.second = i.value();
        cV.append(temppair);
    }
    qSort(cV.begin(), cV.end(), sortDescending);
    displayWords();
}

// displays number of words based on the displayAmt variable
// set by the user
void MainWindow::displayWords()
{
    wc->clear();
    QString showCorrect;
    int show;
    if (displayAmt > cV.size())
        show = cV.size();
    else
        show = displayAmt;

    for (int i = 0; i < show; i++)
    {
        item res = cV.at(i);
        showCorrect += res.first + ": " + QString::number(res.second) + "\n";
    }
    wc->append(showCorrect);
    wc->show();
}
