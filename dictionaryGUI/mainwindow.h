/*
 * mainwindow.h
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 3
 * parekhsm@vt.edu
 * 09/29/2012
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

// forwarded declarations
class QLabel;
class QTextEdit;
class QSplitter;

// typedef for holding QMap items
typedef QPair<QString, int> item;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    // constructor for drawing the main window
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // subwindows created under main window
    // word counter window
    QTextEdit *wc;
    // spell checker window
    QTextEdit *sc;
    
protected slots:
    // Slot function to open a file
    virtual void openFile();
    // Slot function to exit the app
    virtual void exitApp();
    // Slot function to change the amount of frequent
    // words to display
    virtual void amountToDisplay(int);
    // Slot function to change the dictionary language
    virtual void changeLang(int index);

protected:
    // Sets several attributes for the word count &
    // spell check windows
    virtual void createSubWindows();
    // Creates combo box for changing language
    virtual void createComboBox();
    // Creates status bar at the bottom of the window
    virtual void createStatusBar();
    // Loads the file to be checked
    virtual void loadFile(const QString &fileName);
    // Sets the current file as window title
    virtual void setCurrentFile(const QString &fileName);
    // returns the name stripped of the path
    QString strippedName(const QString &fullFileName);
    // spell checks the loaded file
    void spellCheck();
    // counts the most frequent words in the loaded file
    void wordCount();
    // displays the most frequent words
    void displayWords();
    // creates the dictionary
    void createDictionary();

    // variable for holding the file
    QString myFile;
    // variable for holding the status
    QLabel *statusLabel;
private:
    // Main Window
    Ui::MainWindow *ui;
    // Variables for holding the user selected language &
    // text from file
    QString language, text;
    // holds the amount of frequent words to display
    int displayAmt;
    // holds the missed words
    QMap<QString, int> mW;
    // holds the most frequent words
    QMap<QString, int> cW;
    // holds the most frequent words
    QVector<item> cV;
};

#endif // MAINWINDOW_H
