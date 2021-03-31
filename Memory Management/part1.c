#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

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
    unsigned long pageTable[] = { 2, 4, 1, 7, 3, 5, 6 };
    unsigned long pageNumber;
    unsigned long * physicalAddresses;
    unsigned long * ptrStart;
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
      printf("%lu", offset); 
      pageNumber = memAccesses[i] >> 7; //shifting 7 bits to the right to get the page number from logical address
      frameNum = pageTable[pageNumber]; // getting frame number from hardcoded page table
      physicalAddress = (frameNum << 7) + offset; // get the physical address by shiting frame number to the left by 7 bits and then adding offset
      *physicalAddresses = physicalAddress;
      //printf("Physical address %d = %#010lx\n", i, physicalAddress);
      if( write(fd_out, physicalAddresses, sizeof(physicalAddress)) < 0 ) {
        printf("Error!");
        return -1;
      }
      if( i == (filesize/(sizeof( unsigned long )) - 1)) {
        break;
      }
      physicalAddresses++;
	}
    
    close(fd_out);
	free (memAccesses); //free dynamically allocated memory
    physicalAddresses = ptrStart;
    free (physicalAddresses);
}
