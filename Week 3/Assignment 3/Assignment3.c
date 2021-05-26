//
// Created by matt on 5/24/21.
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

/*
* Some of the following could will be inspred from the exmples pofided from Professor Michael Ladreth.
* These files were shared as part of the class and have no directl ink, but will souce
* accordingly.
* Landreth, Michael (12, September 2019) example_signal.c.txt.  Recieved Week 2 Dq2 post.
*/
//make global decs for signals, shared memory buffer, sigset etc.
pid_t globalPid;
sigset_t sigSet;
int WRITE_INPUT = SIGUSR1;
int READ_INPUT = SIGUSR2;
int charCount = 0;
int size = 0;

//define buffer
struct CIRCULAR_BUFFER{
    int max;
    int tail; //updating this from the assignment  Tail and Head make more sense to me
    int head;
    int buffer[25];
};
struct CIRCULAR_BUFFER *inputBuffer = NULL;

//other methods
void signalHandler(int sig){
    printf("Entering the Signal Handler.\n");
}
//create produceer code to read input from user, store each char to an array location and signal the
void writeToBuffer(char input, int size){
    //borrowed from example (Landreth, 2019)  creating the signalset to allow for signals to be handled by this call
    int sig;
    sigemptyset(&sigSet);
    sigaddset(&sigSet, WRITE_INPUT);
    //define next and set a counter to know when all chars have been passed to the funchtion

    int next = inputBuffer->head + 1;

    //check to see if next is not greater than MAX
    if(next > inputBuffer->max){
        next = 0;
    }
    // check to see if next == tail
    if(next == inputBuffer->tail){
        printf("Buffer is full, producer is waiting!\n");
        sigwait(&sigSet, &sig);
    }
    //  Write to buffer
    inputBuffer->buffer[inputBuffer->head] = input;
    printf("Writing %c to the buffer in parent.\n", inputBuffer->buffer[inputBuffer->head]);
    inputBuffer->head = next;
    charCount ++;
    printf("Char Count is: %d\n", charCount);
    if(charCount == size){
        printf("Parent is done writing to the buffer, waking up the child process.\n");
        kill(globalPid, READ_INPUT);
        //exit(0);
    }

}

//create consumer code that waits for a signal from the producer to start consuming the chars on the array and print them to screen.

void readFromBuffer(int size){
    //borrowed from example (Landreth, 2019)  creating the signalset to allow for signals to be handled by this call
    int sig;
    sigemptyset(&sigSet);
    sigaddset(&sigSet, READ_INPUT);
    if(!sigwait(&sigSet, &sig)) {
        size = inputBuffer->head;
        printf("Tail is %c\n", inputBuffer->buffer[inputBuffer->tail]);
        //define next
        int next = inputBuffer->tail + 1;
        //check to see if next is greater than max
        if (next > inputBuffer->max) {
            next = 0;
        }
        //check to make sure tail != head, if so, buffer is empty and read needs to wait for write to happen
        if (next == inputBuffer->head) {
            printf("Buffer is empty, Child process is waiting.");
            sigwait(&sigSet, &sig);
            kill(globalPid, WRITE_INPUT);
        }

        printf("Size in child process is: %d\n", size);
        for (int i = 0; i < size; i++){
            printf("Child process reading from the buffer: %c\n", inputBuffer->buffer[inputBuffer->tail]);
            inputBuffer->tail = i + 1;
        }

    }
    printf("All values read, setting buffer back to null\n");
    inputBuffer = NULL;
    exit(0);
}

int main(int argc, char const *argv[]) {
    pthread_t pid;
    inputBuffer =  (struct CIRCULAR_BUFFER*)mmap(0, sizeof(inputBuffer), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1,0);
    inputBuffer->max = 25;
    inputBuffer->tail = 0;
    inputBuffer->head = 0;
    signal(SIGUSR1, signalHandler);
    signal(SIGUSR2, signalHandler);

    pid = fork();

    if(pid == -1){
        printf("Can't fork, error%d\n", errno);
        exit(EXIT_FAILURE);
    }
    if(pid == 0){
        globalPid = getppid();
        readFromBuffer(size);
    }else{
        //parent  needs to wait for input from user and than read each char of that input into the buffer array.
        globalPid = pid;
        char name[50];
        printf("Please enter your name: \n");
        scanf("%s", name);
        size = strlen(name);
        printf("the size of the array is: %d \n", size);

        for (int i = 0; i < size; i++){
            writeToBuffer(name[i], size);
        }

    }
    wait(NULL);
}
