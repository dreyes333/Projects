#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

void creatPhilosophers(int numberThreads);
void *philosopherThread(void *threadIndex);

void creatPhilosophers(int numberThreads) {
    int i = 0;;
    pthread_t tid[numberThreads];

    for( i= 0; i < numberThreads; i++ ) {
        pthread_create(&tid[i], NULL, philosopherThread, (void *) &i);
    }

    for( i = 0; i < numberThreads; i++ ) {
        (void) pthread_join(tid[i], NULL);
    }
    
    if( i == numberThreads ) {
        printf("%d threads have been completed/joined successfully!\n\n", i);
        return;
    }
}

void *philosopherThread(void *threadIndex) {
    int threadNumber;
    threadNumber = *((int *) threadIndex);
    printf("This is philosopher %d\n", threadNumber);
    return NULL;
}


int main(int argc, char *argv[]) {

    int nthreads;
    
    if(argc != 2) {
        printf("There should be 2 arguments");
        return -1;
        }
    
    nthreads = atoi(argv[1]);
    printf("\nDiego Reyes\n");
    printf("Assignment 4: # of threads = %d\n", nthreads);
    creatPhilosophers(nthreads);

    return 0;
}
