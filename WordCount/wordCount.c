#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

FILE *fileptr;

/*Function that has a fileName pass in and then opens that file based on the filename
Then while fileptr is not at the end of the file you go through the file byte by byte
if char value is a space add 1 to word count 
Finally add one to wordCount because last word doesnt have a space after it and return wordcount of file*/
void countFile( char *fileName ) {
    int wordCount = 0;
    char value;
    char previous = '\n';

    fileptr = fopen(fileName, "r");
    if( fileptr == NULL ) {
        printf("Error: File does not exist\n");
        return;
    }
    while( (value = fgetc(fileptr)) != EOF ) {
        // if current char IS a space OR current char IS a new line AND previous char IS NOT a new line then add to counted words
        if( value == ' ' || value == '\n' && previous != '\n' ) {
            wordCount++;
        }
        previous = value; // set previous to new previous since value will be updated to next value in file
    }
    printf("Child process for %s: number of words in file is %d\n", fileName, wordCount);
    fclose(fileptr);
    return;
}

int main( int argc, char *argv[] ) {

    pid_t forkret;
    int fileCount = argc - 1;
    int i;
    int countret;
    
    printf("\n");

    if( argc <= 1 ) {
        printf("Not enough arguments!\n");
        return 1;
    }
/* for i = 1 ( first argument after executable name until last argument fork the parent so it has multiple 
children if fork == 0 then you are in child process if not you are in parent */
    for( i = 1; i <= fileCount; i++ ) {
        forkret = fork();
        if( forkret == -1 ) {
            perror("Failed to fork!\n");
            return 1;
        }
        if( forkret == 0 ) {
            countFile(argv[i]); // child excutes the countFile function to count # of words
            break;
        } else {
            forkret = wait(NULL); // waiting for the child process to finish terminating 
        }
    }
    
    //Only when you are the parent then print this. Not in for loop because then it prints multiple times.
    if ( forkret > 0 ) {
        printf("\nAll %d files have been counted\n\n", fileCount);
    }

    return 0;
}
