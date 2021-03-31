#include <stdlib.h>
#include <stdio.h>
#include "phypages.h"

unsigned long physicalTable[NUM_FRAMES] = {100,100,100,100,100,100,100,100};
//unsigned long physicalTable[NUM_FRAMES] = {NULL};

unsigned long insertPage(unsigned long pageNumber) { 
    unsigned long i;
    unsigned long count = 7; 

    for ( i = 1 ; i < 8 ; i++ ) {
        if(physicalTable[i] == 100) {
            physicalTable[i] = pageNumber;
            return i;
        }else if(physicalTable[i] != 100 ) {
            count--;
        }
    }
    //if counter == 0 that means array was full so return count 
    if(count == 0) {
        return count;
    }
} 

unsigned long updatePhysicalTable(unsigned long oldPageIndex, unsigned long newPageNum ) {
    
    unsigned long oldPage;

    oldPage = physicalTable[oldPageIndex];
    physicalTable[oldPageIndex] = newPageNum;
    return oldPage;

}
