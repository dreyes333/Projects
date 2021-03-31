#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "phypages.h"
#include "pagetable.h"

typedef struct node {
    unsigned long pNum; //page number
    struct node * next;
} recent_t;

// Create a new instance of the object
static recent_t * createObject( unsigned long pnum) { 
    recent_t *obj = (recent_t *) malloc(sizeof(recent_t));
    if( obj == NULL ) {
        return;
    }
    obj->pNum = pnum;
    obj->next = NULL;
    return obj;
}

//adds a node to the front of linked list
void addToFront(recent_t ** head, unsigned long pnum) {
    recent_t * obj;
    obj = malloc(sizeof(recent_t));
    obj->pNum = pnum;
    obj->next = *head;
    *head = obj;
}

//function that starts iterating through linked list from beggining
unsigned long findLRU( recent_t * head ) {
    recent_t * current = head;
    unsigned long checkLast[8] = {0,0,0,0,0,0,0,0}; //array that keeps track of found values by setting bits, ignore 0th index
    unsigned long i; 
    int counter = 0; //counts number of elements that matched
    unsigned long frameIndex; //return value that will be index of pageNum you want to replace
    
    current = current->next; 
    while( current != NULL ) {//iterating through linked list
     //printf("HELLO %lu", current->pNum);
        for( i = 1 ; i < 8 ; i++ ) { //iterating through frame table 
            
            //if value in node is equal to the page number in the frame table but checkLast[i] has been set already:found duplicate value, break out of forloop
            if( current->pNum == physicalTable[i] && checkLast[i] != 0 ) { 
                break;
            }else if( current->pNum == physicalTable[i] && checkLast[i] == 0 ) { //elseif checkLast[i] is not set, found value not duplicate
                checkLast[i] = 1; //set to 1
                if(counter <  6) { 
                    counter++; 
                } else {
                    frameIndex = i;
                    return frameIndex;
                }
                break; //found value break out of for loop continue with while 
            }else if( current->pNum == physicalTable[i] && counter == 6 && checkLast[i] == 0 ) { //found least recently used value, return the index of that value
                frameIndex = i;
                //printf("THIS IS THE VALUE YOU WILL RETURN LRU %lu", frameIndex);
                return frameIndex;
            }else{ //current->next != frameTable[i], go on to next loop, better luck next iteration
                continue;
            }
        }
     current = current->next;
    }
    //for( i = 0; i < 8 ; i++ ) {
    //    printf(" %lu ", checkLast[i]);
    //}
    //printf("THIS IS THE COUNER %d", counter); 
    //printf("CASES WERE NOT MET!");
}

int main(int argc, char ** argv) {
    char *infile;
    char *outfile;
	int fd_in;
    int fd_out;
	struct stat st;
	unsigned long filesize;
	unsigned long * memAccesses;
	int i; 

    unsigned long offset;
    unsigned long frameNum;
    unsigned long physicalAddress;
    unsigned long pageNumber;
    unsigned long * physicalAddresses;
    unsigned long * ptrStart;
        
    unsigned long foundLRUframe;
    unsigned long checkPageRet;
    recent_t * new = NULL;
    unsigned long oldPageRet;

	if(argc != 3) {
		fprintf(stderr, "Usage: %s SequenceFile\n", argv[0]);
		exit(0); 
	}
	infile = argv[1]; 
    outfile = argv[2];
	stat(infile, &st);
	filesize = st.st_size;
	//allocate space for all addresses
	memAccesses = (unsigned long*) malloc( (size_t) filesize );
    physicalAddresses = (unsigned long*) malloc( (size_t) filesize );
    //use open and read
	fd_in = open(infile, O_RDONLY);
	if(fd_in == -1) {
		fprintf(stderr, "fd is invalid, with error %d\n", strerror(errno));
		exit(-1);
	}
	//read all bytes from file at once without check errors !!!
	read(fd_in, memAccesses, (int) filesize);
	close(fd_in);
    
    //open and write with all privilges set
    fd_out = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 00777);
    if(fd_out == -1) {
        fprintf(stderr, "fd is invalid, with error %d\n", strerror(errno));
        exit(-1);
    }
    ptrStart = physicalAddresses;
	// Traverse all address
	for(i = 0; i < filesize/(sizeof (unsigned long)) ; i++) {
	  //printf("\nLogical address %d = %#010lx\n", i, memAccesses[i]);
      offset = memAccesses[i] & 0x007F; //copying last byte from logical address to get offset
      pageNumber = memAccesses[i] >> 7; //shifting 7 bits to the right to get the page number from logical address
      //creates a new node on first iteration, after that it just creates and adds nodes
      if( i == 0 ) {
        new = createObject(pageNumber);
      } else {
        addToFront(&new, pageNumber);
      } 
      
      //call function that passes pagenumber as parameter and then checks the pagetable index to see if something is in there already
      checkPageRet = checkPageTable( pageNumber );

      // if checkPageRet == 0 then insert the page into frame table
      if(checkPageRet == 0) {
        frameNum = insertPage(pageNumber);
        if(frameNum != 0) {
            //printf("\n %lu %lu \n", pageNumber, frameNum); //TEST COMMENT OUT LATER
            insertPageTable(frameNum, pageNumber);
        } else {
          //printf("\n %lu %lu HOLA  \n", pageNumber, frameNum);
          foundLRUframe = findLRU( new );
          //printf("\nLRU RETURN IS : %lu \n", foundLRUframe);    
          oldPageRet = updatePhysicalTable( foundLRUframe, pageNumber );
          updatePageTable( foundLRUframe , pageNumber, oldPageRet );
        }
      } else if(checkPageRet != 0 && physicalTable[checkPageRet] == pageNumber && pageTable[pageNumber] == checkPageRet) {
        //printf("Value already mapped dont do anything!");
        printf("\n");
      }

      frameNum = pageTable[pageNumber]; // getting frame number from hardcoded page table
      physicalAddress = (frameNum << 7) + offset; // get the physical address by shiting frame number to the left by 7 bits and then adding offset
      *physicalAddresses = physicalAddress;
      //printf("Physical address %d = %#010lx\n", i, physicalAddress);
      if( write(fd_out, physicalAddresses, sizeof(physicalAddress)) < 0 ) {
        printf("Error!");
        return -1;
      }
      //checks if i is in its last iteration and if it is it breaks
      if( i == (filesize/(sizeof( unsigned long )) - 1)) {
        break;
      }
      physicalAddresses++;
	}
    
    close(fd_out);
	free (memAccesses); //free dynamically allocated memory
    physicalAddresses = ptrStart; // set physicalAddresses to the beggining so that you can free it
    free (physicalAddresses);
}
