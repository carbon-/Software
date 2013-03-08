/*
 * Dictionary.h
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 3
 * parekhsm@vt.edu
 * 09/13/2012
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <QFile>
#include <QStringList>
#include <QString>
#include <QMap>
#include <QDebug>


class Dictionary
{
private:
	// Dictionary file is stored as a string list
	QStringList dictionary;
	// stores misspelled words
	QMap<QString, int> spellChecker;
	// stores correct words
	QMap<QString, int> wordCounter;

public:
	Dictionary(QString file);					// constructor
	~Dictionary();								// destructor

	QMap<QString, int> getMissedWords();		// returns spellChecker
	QMap<QString, int> getWordCount();			// returns wordCount
	void isMisspelled(QStringList words);		// creates maps for missed and correct words
};


#endif /* DICTIONARY_H_ */
