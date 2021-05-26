//
// Created by matt on 5/25/21.
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

//declare globals
int counter = 0;
int misses = 0;
time_t time_to_run;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increaseCounter(){
    printf("Entering the increase counter function\n");
    //the counter needs to sleep for 1, increase the counter by one and sleep 1 again all within a mutex lock
    while(time(NULL) < time_to_run){
        pthread_mutex_lock(&mutex);
        counter++;
        sleep(1);
        pthread_mutex_unlock(&mutex);
        printf("Counter %d\n", counter);
    }
    pthread_exit(0);
}

void* monitorCounter(){
    //Needs to check the counter every 3 seconds and lock it if it can.  If the counter is locked, increase missed by 1
    //print out the coutner if it can, print misses when it exits
    while (time(NULL) < time_to_run){
        sleep(3);
        int lockStatus = pthread_mutex_trylock(&mutex);
        printf("The status is: %d\n", lockStatus);
        if(lockStatus == 0){
            printf("Monitor locked the counter, it's current count is: %d\n", counter);
            pthread_mutex_unlock(&mutex);
        }else{
            misses ++;
        }
    }
    pthread_exit(0);

}

int main(int argc, char const *argv[]) {
    int inputTime;
    printf("Please enter a length of time for the program to run.\n");
    scanf("%d", &inputTime);
    time_to_run = time(NULL) + inputTime;

    pthread_t counterThreadId;
    pthread_create(&counterThreadId, NULL, increaseCounter, NULL);

    pthread_t monitorThreadId;
    pthread_create(&monitorThreadId, NULL, monitorCounter, NULL);

    pthread_join(counterThreadId, NULL);
    printf("Final count was: %d\n", counter);
    pthread_join(monitorThreadId, NULL);
    printf("Total misses is: %d\n", misses);
}
