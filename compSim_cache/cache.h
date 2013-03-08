/*
 * cache.h
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 5
 * parekhsm@vt.edu
 * 10/29/2012
 */

#ifndef CACHE_H
#define CACHE_H
#include "control.h"
#include "memory.h"

#define TAG16MASK 0xF0
#define TAG32MASK 0xE0
#define TAG64MASK 0xC0

// colors for updating GUI
enum colors
{
    white,
    red,
    green
};

struct block
{
    int valid;
    int tag;
    int dirty;
};

class cache : public QObject
{
    Q_OBJECT

private:
    int hits;                                               // stores number of hits
    int misses;                                             // stores number of misses
    int cacheSize;                                          // stores cache size
    int blockSize;                                          // stores block size
    int rows;                                               // stores rows
    block* blocks;                                          // stores blocks
    word* cacheEntry;                                       // stores cache entries
    word data;                                              // holds data from memory
    word addr;


public slots:
    void changeCacheSize(int);                              // changes cache size
    void changeBlockSize(int);                              // changes block size
    void writeToCache(word newData, word address);          // for STORE, STOREA and ADD
    void readFromCache(word address);                       // for LOAD, LOADA
    void setData(word newData);                             // sets data in cache to newData
    void flushCache();                                      // flushes the cache

    void resetCache();                                      // resets the cache
    void clearCache();                                      // clears the cache

public:
    explicit cache(QObject *parent = 0);
    ~cache();
    void createCache();                                     // creates the cache
    int getTag(word address);                               // returns the tag
    int getIndex(word address);                             // returns the index
    int getOffset(word address);                            // returns the offset
    int log2(int x);                                        // returns the log2

signals:
    void returnRows(int);                                   // returns rows to GUI
    void returnBlocks(int);                                 // returns blocks to GUI
    void updateRatio(double);                               // updates ration on GUI
    void updateHits(int);                                   // updates hits on GUI
    void setAccumulator(word);                              // sets the accumulator to value
    void setTableColor(int color);                          // sets the appropriate color in instruction table
    void readBlockFromMem(word address);                    // reads a block from memory
    void writeBlockToMem(word value, word address);         // writes a block to memory
    void updateGUI(int value, int tag, int valid, int row, int column);     // updates GUI

};



#endif // CACHE_H
