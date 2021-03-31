#ifndef PP_H_
#define PP_H_
#define NUM_FRAMES 8
extern unsigned long physicalTable[NUM_FRAMES];

unsigned long insertPage(unsigned long pageNumber);
unsigned long updatePhysicalTable(unsigned long oldPageIndex, unsigned long newPageNum );

#endif
