#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pslibrary.h"

int main( int argc, char* argv[] ) {
    int i = 1; 
    char fcfs1[100];
    char fcfs2[100];
    char sjf1[100];
    char sjf2[100];
    char psjf1[100];
    char psjf2[100];
    char rr1[100];
    char rr2[100];
    if(argc != 8) {
        printf("\nNot enough arguments, need 7!\n");
        exit(0);
    } else {
        printf("\nAssignment 2 program was written by Diego Reyes\n");
        printf("Arguments: ");
        for(i = 1; i <= argc - 1; i++ ) {
            printf(" %d ", atoi(argv[i]));
        }
        printf("\n");
    }

    //TODO: Call each of the 4 functions one at a time
    fcfs(fcfs1,fcfs2,atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),atoi(argv[6]),atoi(argv[7]));
    schedulerInfo(fcfs1,fcfs2,"Scheduler FCFS");    
    sjf(sjf1,sjf2,atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),atoi(argv[6]),atoi(argv[7]));
    schedulerInfo(sjf1,sjf2,"Scheduler SJF");
    psjf(psjf1,psjf2,atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),atoi(argv[6]),atoi(argv[7]));
    schedulerInfo(psjf1,psjf2,"Scheduler PSJF");
    rr(rr1,rr2,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),atoi(argv[6]),atoi(argv[7]));
    schedulerInfo(rr1,rr2,"Scheduler RR");
    printf("\n");
    return 0;
}
