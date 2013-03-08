/*
 * Dictionary.cpp
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 3
 * parekhsm@vt.edu
 * 09/13/2012
 */
#include "Dictionary.h"

QTextStream cout(stdout);

// Constructor
Dictionary::Dictionary(QString file)
{
    // open the dictionary file and store the words in a string list
    QFile myFile(file);
    if (myFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream infile(&myFile);
        QString line = infile.readAll();
        dictionary.clear();
        dictionary = line.split(QRegExp("\\s+"));
    }
    else
    {
        cout << "File not found\n!";
    }
    myFile.close();
}

// Destructor
Dictionary::~Dictionary()
{

}

// Function goes through the words from the text and checks
// if they are in the dictionary, if they are it inserts the word
// to wordCounter(which maps correct words) else it inserts them
// to spellChecker(which maps misspelled words).
void Dictionary::isMisspelled(QStringList words)
{
    spellChecker.clear();
    wordCounter.clear();
    int i = 0;
    // go through the text and check it against the dictionary
    for (i = 0; i < words.size(); i++)
    {
        // if dictionary does not contain the word in text put it in
        // spellChecker map (which contains the misspelled words)
        // also update the count
        if(!dictionary.contains(words[i], Qt::CaseInsensitive))
        {
            int count = spellChecker.value(words[i]);
            count += 1;
            spellChecker.insert(words[i], count);
        }
        // if the word is contained in the dictionary put it in the
        // wordCounter map (which contains correct words) and update
        // the count
        else
        {
            int count = wordCounter.value(words[i]);
            count += 1;
            wordCounter.insert(words[i], count);
        }
    }
}

// return misspelled words
QMap<QString, int> Dictionary::getMissedWords()
{
    return spellChecker;
}

// return correct words
QMap<QString, int> Dictionary::getWordCount()
{
    return wordCounter;
}
