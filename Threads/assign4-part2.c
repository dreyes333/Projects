#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include<time.h>

pthread_mutex_t *chopsticks;
int max = 0;

void creatPhilosophers(int numberThreads);
void *philosopherThread(void *threadIndex);
void thinking(int numberThreads);
void pickUpChopsticks(int threadIndex);
void eating(int numberThreads);
void putDownChopsticks(int threadIndex);

void thinking(int threadNum) {
    printf("Philosopher #-%d: started thinking\n", threadNum);
    srand(time(0));
    usleep( rand()%(500+1-1)+1 ); //thread safe
    printf("Philosopher #-%d: end thinking\n", threadNum);
}

void pickUpChopsticks(int threadIndex) {
    
    pthread_mutex_lock(&chopsticks[threadIndex]); 
    int random = rand() % 500;
    usleep(random);
    pthread_mutex_lock(&chopsticks[(threadIndex+1)%max]);

}

void eating(int threadNum) {
    printf("Philosopher #-%d: started eating\n", threadNum);
    srand(time(0));
    usleep( rand()%(500+1-1)+1 ); //thread safe 
    printf("Philosopher #-%d: end eating\n", threadNum);
}

void putDownChopsticks(int threadIndex) {

    pthread_mutex_unlock(&chopsticks[threadIndex]); 
    int random = rand() % 500;
    usleep(random);
    pthread_mutex_unlock(&chopsticks[(threadIndex+1)%max]);

}

void creatPhilosophers(int numberThreads) {
    int i = 0;
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

    thinking(threadNumber);
    pickUpChopsticks(threadNumber);
    eating(threadNumber);
    putDownChopsticks(threadNumber);

    pthread_exit(NULL);
    //printf("This is philosopher %d\n", threadNumber);
    
}


int main(int argc, char *argv[]) {

    int nthreads;
    int count = 0;

    if(argc != 2) {
        printf("\nThere should be 2 arguments\n");
        return -1;
        }
    
    nthreads = atoi(argv[1]);
    max = nthreads;
    chopsticks = (pthread_mutex_t *)malloc(nthreads*sizeof(pthread_mutex_t));

    //pthread_mutex_t chopsticks = PTHREAD_MUTEX_INITIALIZER;
    for( count = 0; count < nthreads; count++ ) {
        pthread_mutex_init(&chopsticks[count],NULL);
    }

    printf("\nDiego Reyes\n");
    printf("Assignment 4: # of threads = %d\n", nthreads);
    creatPhilosophers(nthreads);
    
    free(chopsticks);

    return 0;
}
