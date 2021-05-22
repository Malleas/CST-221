#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <signal.h>
#include "sys/wait.h"

/*
* Some of the following could will be inspred from the exmples pofided from Professor Michael Ladreth.
* These files were shared as part of the class and have no directl ink, but will souce
* accordingly.
* Landreth, Michael (12, September 2019) example_signal.c.txt.  Recieved Week 2 Dq2 post.
*/

//define my signals and any other globals i need.

pid_t globalPid;
int WRITE_TO_BUFFER = SIGUSR1;
int READ_FROM_BUFFER = SIGUSR2;
sigset_t sigSet;

void sigHandler(int sig){
    printf("Entering the Sig Handler \n");
}
//define my Buffer
struct CIRCULAR_BUFFER{
    int max;
    int tail; //updating this from the assignment  Tail and Head make more sense to me
    int head;
    int buffer[100];
};
struct CIRCULAR_BUFFER *buffer = NULL;
//get() some value from the head of the dataset

//put() some value into the dataset
void writeToBuffer(int inNum){
    //borrowed from example (Landreth, 2019)  creating the signalset to allow for signals to be handled by this call
    int sig;
    sigemptyset(&sigSet);
    sigaddset(&sigSet, WRITE_TO_BUFFER);

    //define next write location
    int next = buffer->head + 1;
    //if next is > MAX, next == 0
    if(next > buffer->max){
        next = 0;
    }
    //check to see if next == tail, if so, send signal that the buffer is full and produce needs to sleep
    if(next == buffer->tail){
        printf("Buffer is full, producer pausing. \n");
        sigwait(&sigSet, &sig); //waiting for signal from consumer that it has written and freed up some space on the buffer.
    }
    //if next is not eithe of the above, write inNum to next and update head to next
    buffer->buffer[next] = inNum;
    buffer->head = next;
    printf("Wrote %d to the buffer at location: %d\n", inNum, buffer->head);
    kill(globalPid, READ_FROM_BUFFER);

}

void readFromBuffer(){
    //borrowed from example (Landreth, 2019)  creating the signalset to allow for signals to be handled by this call
    int sig;
    sigemptyset(&sigSet);
    sigaddset(&sigSet, READ_FROM_BUFFER);

    //define next
    int next = buffer->tail + 1;

    //check to see if next is > max, if so set to 0
    if(next > buffer->max){
        next = 0;
    }
    //check to see if head == tail, if so, buffer is empty and consumer needs to wait for producer to send signal.
    if(buffer->head == buffer->tail){
        printf("Buffer is empty, nothing to read.\n");

    }
    //print the number from the buffer to the scree, update tail
    sigwait(&sigSet, &sig);
    printf("Reading from the buffer, the current buffer number is: %d\n", buffer->tail);
    buffer->tail = next;
    kill(globalPid, WRITE_TO_BUFFER);

}


int main(int argc, char const *argv[]) {
  pid_t pid;

  buffer =  (struct CIRCULAR_BUFFER*)mmap(0, sizeof(buffer), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1,0);

  buffer->max = 100;
  buffer->tail = 0;
  buffer->head = 0;

  signal(SIGUSR1, sigHandler);
  signal(SIGUSR2, sigHandler);

  pid = fork();
  if(pid == -1){
    printf("Can't fork, error%d\n", errno);
    exit(EXIT_FAILURE);
  }
  if(pid == 0){
      globalPid = getppid();
    //Start for loop with a method that adds data to the buffer
    for (int i = 0; i < 500; i++){
        writeToBuffer(i);
    }
  } else{
      globalPid = pid;
      for (int i = 0; i < 500; i++) {
          readFromBuffer();
      }
      //sleep till signal is sent then do read what ever the tail marker is of the data set to printf
  }
  wait(NULL);
  wait(NULL);
  return 0;
}
