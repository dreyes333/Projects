#ifndef PT_H_
#define PT_H_
#define NUM_PAGES 32

extern unsigned long pageTable[NUM_PAGES];

unsigned long checkPageTable( unsigned long pageNum );
void updatePageTable( unsigned long frameNum, unsigned long pageTableIndex, unsigned long oldPage );
void insertPageTable(unsigned long frameNum, unsigned long pageNumber);

#endif
