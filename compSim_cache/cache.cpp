/*
 * cache.cpp
 *
 * Name: Shaishav Parekh
 * ECE 3574
 * 905448581
 * Homework 5
 * parekhsm@vt.edu
 * 10/19/2012
 */

#include "cache.h"
#include <QDebug>


// create an empty cache
cache::cache(QObject *parent)
{
    hits = 0;
    misses = 0;
    cacheSize = 16;
    blockSize = 2;
    rows = cacheSize/blockSize;
    createCache();
}

cache::~cache()
{}

// change the block size to match GUI
void cache::changeBlockSize(int newBSize)
{
    if (newBSize == 0)
        blockSize = 2;
    else if (newBSize == 1)
        blockSize = 4;
    else
        blockSize = 8;
    rows = cacheSize/blockSize;
    // recreate cache
    createCache();
}

// change the cache size to match GUI
void cache::changeCacheSize(int newCSize)
{
    if (newCSize == 0)
        cacheSize = 16;
    else if (newCSize == 1)
        cacheSize = 32;
    else
        cacheSize = 64;
    rows = cacheSize/blockSize;
    // recreate cache
    createCache();
}

// create a cache memory
void cache::createCache()
{
    // cache has rows * blockSize space
    cacheEntry = new word[rows * blockSize];
    // create blocks for each row in the cache
    blocks = new block[rows];
    for (int i = 0; i < rows; i++)
    {
        blocks[i].dirty = 0;
        blocks[i].valid = 0;
        blocks[i].tag = 0;
    }
    // update the GUI
    emit returnRows(rows);
    emit returnBlocks(blockSize);
}
// WRITE to cache whenever a store, storea or add instruction is called
void cache::writeToCache(word newData, word address)
{
    int tag, iindex, offset;
    block wblock;

    // get tag, index, and offset
    tag = getTag(address);
    iindex = getIndex(address);
    offset = getOffset(address);


//    qDebug() << tag;
//    qDebug() << iindex;
//    qDebug() << offset;


    wblock = blocks[iindex];
    // check if block is valid and tag matches
    if ((wblock.valid == 1) && (wblock.tag == tag))
    {
        // if it does increment hits
        hits++;
        // set the cache to new data
        cacheEntry[rows*iindex + offset] = newData;
        // mark it dirty
        wblock.dirty = 1;
        // cache hit on GUI
        emit setTableColor(green);
    }
    else
    {
        // if it doesnt match increment misses
        misses++;
        // write back old value if it is dirty
        if (wblock.dirty == 1)
        {
            word value = cacheEntry[iindex*rows + offset];

            word raddr = blocks[iindex].tag + iindex + offset;
            emit writeBlockToMem(value, raddr);
        }
        //  get new value, replace old value with new
        emit readBlockFromMem(address);

        cacheEntry[rows*iindex + offset] = newData;

        // mark it dirty
        wblock.tag = tag;
        wblock.dirty = 1;
        wblock.valid = 1;

        // set the color to red since it was a miss
        emit setTableColor(red);

    }
    // set the block with tag, valid and diry
    blocks[iindex] = wblock;

    // update ratio and hits on GUI
    double ratio = double(hits)/(double(hits)+double(misses));
    emit updateRatio(ratio);
    emit updateHits(hits);

    // update cache on GUI
    emit updateGUI(newData, tag, wblock.valid, iindex, offset);
}

// READ from cache, called whenever LOAD, LOADA instruction is executed
void cache::readFromCache(word address)
{

    int tag, iindex, offset;
    block rblock;
    // get tag, index and offset
    tag = getTag(address);
    iindex = getIndex(address);
    offset = getOffset(address);

    rblock = blocks[iindex];

    // if block is valid and tag matches set the accumulator
    if ((rblock.valid == 1) && (rblock.tag == tag))
    {
        // increment misses
        hits++;
        // set the accumulator to value in cache
        data = cacheEntry[iindex*rows + offset];

        // color table cell green
        emit setTableColor(green);
    }
    else
    {
        // increment misses
        misses++;
        // read the value from memory
        // if the value is dirty write it back
        if (rblock.dirty == 1)
        {
            word value = cacheEntry[iindex*rows + offset];

            // not the right address to right to, since this
            // is the new address thats not in the cache
            word raddr = blocks[iindex].tag + iindex + offset;
            emit writeBlockToMem(value, raddr);


        }
        // read new data from address
        emit readBlockFromMem(address);

        // reset dirty and valid, set tag to new tag
        rblock.tag = tag;
        rblock.dirty = 0;
        rblock.valid = 1;
        // put block back in cache
        blocks[iindex] = rblock;

        // color table cell red since it was a miss
        emit setTableColor(red);

    }
    // set accumulator to new data
    emit setAccumulator(data);

    // update ratio on GUI
    double ratio = double(hits)/(double(hits)+double(misses));
    emit updateRatio(ratio);
    emit updateHits(hits);

    // update cache GUI
    emit updateGUI(data, tag, rblock.valid, iindex, offset);
}

// Function to set data whenever data is read from memory
void cache::setData(word newData)
{
    data = newData;
}

// gets the tag based on cache size
int cache::getTag(word address)
{
    int tag;

    if (cacheSize == 16)
    {
        tag = address & TAG16MASK;
    }
    else if (cacheSize == 32)
    {
        tag = address & TAG32MASK;
    }
    else
    {
        tag = address & TAG64MASK;
    }

    return tag;
}

// gets the index based on block size
int cache::getIndex(word address)
{
    int iindex;
    if (blockSize == 2)
    {
        iindex = address & 0x0E;
    }
    else if (blockSize == 4)
    {
        iindex = address & 0x0C;
    }
    else
    {
        iindex = address & 0x08;
    }
    iindex = iindex >> log2(blockSize);
    return iindex;
}

// gets the offset based on block size
int cache::getOffset(word address)
{
    int offset;
    if (blockSize == 2)
    {
        offset = address & 0x01;
    }
    else if (blockSize == 4)
    {
        offset = address & 0x03;
    }
    else
    {
        offset = address & 0x07;
    }

    return offset;
}

// calculates the log2 of a number
int cache::log2(int x)
{
    int count = 0;
    while (x > 1)
    {
        if ((x % 2) == 0)
        {
            x = x/2;
            count++;
        }
        else
            return -1;
    }
    return count;
}

// flush the cache when halt is reached
void cache::flushCache()
{

}

// reset the cache, doesnt reset hits, misses
void cache::resetCache()
{
    for (int i = 0; i < (rows*blockSize); i++)
    {
        cacheEntry[i] = 0;
    }
    for (int i = 0; i < rows; i++)
    {
        blocks[i].dirty = 0;
        blocks[i].tag = 0;
        blocks[i].valid = 0;
    }
    data = 0;
    addr = 0;
}

// clears the cache, clears hits, misses
void cache::clearCache()
{
    resetCache();
    hits = 0;
    misses = 0;
    data = 0;
    addr = 0;
    emit updateHits(hits);
    emit updateRatio(0);
}

