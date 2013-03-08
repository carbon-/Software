/*
 * main.cpp for spellCheck
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
#include "Dictionary.h"

int main(int argc, char *argv[]) {
	QApplication myApp(argc, argv);
	if (argc != 3)
	{
		qDebug() << "Error: invalid usage. \nUsage: <file> <language>";
		return 1;
	}
	// initialize a widget for outputting the results
	QTextEdit myWidget;
	myWidget.setMinimumSize(500,300);
	myWidget.setAcceptRichText(true);

	// File input variables
	QStringList inWords;
	QString text;

	// Map for missed words
	QMap<QString, int> missedList;

	// Input words from file, get rid of punctuation
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
		// create the misspelled word map
		myAmericanDict.isMisspelled(inWords);
		// output the map
		missedList = myAmericanDict.getMissedWords();
	}
	else if (myApp.arguments()[2] == "british")
	{
		QString file = QString("/usr/share/dict/british-english");
		// initialize the dictionary
		Dictionary myBritishDict(file);
		// create the misspelled word map
		myBritishDict.isMisspelled(inWords);
		// output the map
		missedList = myBritishDict.getMissedWords();
	}
	// if language specified is not an option, output error and exit
	else
	{
		qDebug() << "Error: User specified dictionary not found!";
		return 1;
	}
	// variables for formatting misspelled words
	QString sred = "<font color = '#ff0000'>";
	QString ered = "</font>";
	QString temp;

	// go through the missedspelled words and color the word
	// in text red
	foreach(const QString &str, missedList.keys())
	{
		if (text.contains(str))
		{
			temp = sred+str+ered;
			text.replace(str, temp);
		}
	}
	// append and show the formatted text
	myWidget.append(text);
	myWidget.show();
	return myApp.exec();
	//return 0;
}


