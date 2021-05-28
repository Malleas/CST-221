//
// Created by matt on 5/27/21.
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>
#include <sys/wait.h>

//globlas
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pid_t globalPid;
pid_t childPid;
int threadNum = 2;
//Define a buffer
struct BUFFER{
    long buffer[25];
};
struct BUFFER *pidBuffer = NULL;
//child process
void* someFunc(void *arg){
    //sleep(rand()%(2-0+1)+1);

    //long tid = *(long *) arg;
    //pthread_mutex_lock(&mutex);
    int index = *(int *) arg;
    printf("In thread %d someFunc()\n", index);
    //pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

void childProcess(){
    //create some logic to spin up three counters that are numbered and wait for a flag to exist otherwise continue to count
    //each one should have a differen sleep and access a different counter.
   int somePid = getpid();
    printf("Child process pid: %d\n", somePid);
    pthread_t tid;
    for (int i = 0; i <threadNum ; ++i) {

        printf("The tid in child process is: %ld\n", tid);

        int index = i;
        printf("index is: %d\n", index);
        pthread_create(&tid, NULL, someFunc, &index);
        pidBuffer->buffer[i] = tid;

    }
    sleep(2);

}
//parent process
void parentProcess(){
    // accept an int signature and use that to send a kill signal to a given child process


}

int main(int argc, char const *argv[]) {
    pid_t pid;
    pidBuffer =  (struct BUFFER*)mmap(0, sizeof (struct BUFFER), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1,0);
    pid = fork();


    if(pid == -1){
        printf("Can't fork, error%d\n", errno);
        exit(EXIT_FAILURE);
    }

    if(pid == 0 ){
        //child process
        int childPid = getpid();
        int parentId = getppid();
        //printf("Child pid in main is: %d and the parentId is: %d\n", childPid, parentId);
        childPid = getppid();
        childProcess();
    } else{
        //parent process
        int parentPid = getpid();
        //printf("The pid int he parent process is: %d\n", parentPid);
        globalPid = pid;
        parentProcess();



    }
    //pthread_join(childPid, NULL);
}
