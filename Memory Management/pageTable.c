#include "pagetable.h"

unsigned long pageTable[NUM_PAGES] = { };

//TODO: PAGEFAULT COUNT
unsigned long checkPageTable( unsigned long pageNum ) {
    
    unsigned long frameNum;
    unsigned long noFrame = 0;

    //if pageTable entry has no frame in it return pageNum so it can be inserted in frameTable
    if( pageTable[pageNum] == 0 ) {
        return noFrame;
    //if pageTable entry currently has a frame then return frameNum
    } else if ( pageTable[pageNum] != 0 ) {
        frameNum = pageTable[pageNum];
        return frameNum;
    }     
}

void updatePageTable( unsigned long frameNum, unsigned long pageTableIndex, unsigned long oldPage ) {
    
    pageTable[oldPage] = 0;
    pageTable[pageTableIndex] = frameNum;

}

void insertPageTable(unsigned long frameNum, unsigned long pageNumber) {

    pageTable[pageNumber] = frameNum;

}

