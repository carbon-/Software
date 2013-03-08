/*
 * main.cpp for wordCount
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 3
 * parekhsm@vt.edu
 * 09/13/2012
 */

#include <QtCore>
#include <QtGui>
#include <QCoreApplication>
#include <QWidget>
#include <QtGlobal>
#include <QVector>
#include <QPair>
#include <QtAlgorithms>
#include "Dictionary.h"

// Pair to hold items from maps
typedef QPair<QString, int> item;

// Sort function to sort a list of pairs in descending order
bool sortDescending(item& aword, item& bword)
{
	if(aword.second < bword.second)
		return false;
	else
		return true;
}

int main(int argc, char *argv[])
{
	QApplication myApp(argc, argv);

	// if 4 arguments are not defined, display error and exit
	if (argc != 4)
	{
		qDebug() << "Error: invalid usage.\nUsage: <file> <language> <int>";
		return 1;
	}
	// initialize two widgets, one for showing correct words, another for showing misspelled
	QTextEdit Widget1, Widget2;
	Widget1.setMinimumSize(500,300);
	Widget2.setMinimumSize(500,300);

	// File input variables
	QStringList inWords;
	QString text;

	// initialize two maps to hold the missed and correct words
	QMap<QString, int> missedList;
	QMap<QString, int> correctList;

	// Input words from file, get rid of any punctuation
	QFile inFile(myApp.arguments()[1]);
	if (inFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&inFile);
		text = in.readAll();
		inWords = text.split(QRegExp("\\W+"), QString::SkipEmptyParts);
	}
	else
	{
		qDebug() << "Error: File could not be found OR doesn't exist!";
		return 1;
	}
	// if language is american use the american-english dictionary
	if (myApp.arguments()[2] == "american")
	{
		QString file = QString("/usr/share/dict/american-english");
		// initialize the dictionary
		Dictionary myAmericanDict(file);
		// create the misspelled and correct word maps
		myAmericanDict.isMisspelled(inWords);
		// output maps
		missedList = myAmericanDict.getMissedWords();
		correctList = myAmericanDict.getWordCount();
	}
	// if language is british use the british-english dictionary
	else if (myApp.arguments()[2] == "british")
	{
		QString file = QString("/usr/share/dict/british-english");
		// initialize the dictionary
		Dictionary myBritishDict(file);
		// create the misspelled and correct word maps
		myBritishDict.isMisspelled(inWords);
		// output maps
		missedList = myBritishDict.getMissedWords();
		correctList = myBritishDict.getWordCount();
	}
	// if language specified is not an option, output error and exit
	else
	{
		qDebug() << "Error: User specified dictionary not found!";
		return 1;
	}
	// initialize two vectors to sort the words
	QVector< item > missedVector;
	QVector< item > correctVector;

	// go through the maps and add the words to the vectors for the missedList
	// and correctList
	QMapIterator<QString, int> i(missedList);
	while(i.hasNext())
	{
		i.next();
		item temppair;
		temppair.first = i.key();
		temppair.second = i.value();
		missedVector.append(temppair);
	}
	QMapIterator<QString, int> j(correctList);
	while(j.hasNext())
	{
		j.next();
		item temppair;
		temppair.first = j.key();
		temppair.second = j.value();
		correctVector.append(temppair);
	}

	// Sort the vectors is descending order
	qSort(correctVector.begin(), correctVector.end(), sortDescending);
	qSort(missedVector.begin(), missedVector.end(), sortDescending);

	// Two strings to show the result in window
	QString showcorrect, showmissed;

	// get the number of items to show
	int amttoshow = myApp.arguments()[3].toInt();
	if (amttoshow <= 0)
	{
		qDebug() << "Error: <int> is invalid!";
		return 1;
	}
		
	int index;

	// check if amount > amount of words in list
	// if the amount is greater then the size of list is used
	// else the user-specified amount is used

	if (amttoshow > correctVector.size())
		index = correctVector.size();
	else
		index = amttoshow;

	// put the words in a string
	for (int k = 0; k < index; k++)
	{
		item result = correctVector.at(k);
		showcorrect += result.first + ": " + QString::number(result.second) + "\n";
	}

	// check if amount > amount of words in list
	// if the amount is greater then the size of list is used
	// else the user-specified amount is used
	if (amttoshow > missedVector.size())
		index = missedVector.size();
	else
		index = amttoshow;

	// put the results in a string
	for (int k = 0; k < index; k++)
	{
		item result = missedVector.at(k);
		showmissed += result.first + ": " + QString::number(result.second) + "\n";
	}

	// Output the results to their respective windows
	Widget1.setWindowTitle("List of correct words");
	Widget2.setWindowTitle("List of misspelled words");
	Widget1.append(showcorrect);
	Widget2.append(showmissed);
	Widget1.show();
	Widget2.show();
	return myApp.exec();
	//return 0;
}



