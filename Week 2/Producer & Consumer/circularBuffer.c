#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/mman.h>


struct CIRCULAR_BUFFER{
  int * count;
  int * lower;
  int * upper;
  int * buffer[100];
};
struct CIRCULAR_BUFFER *buffer = NULL;


static STOP_PROD = 0;
static WAKE_CONSUMER = 1;

void prod_handler(int sig){
  if(sig = SIGUSR1){
    STOP_PROD = 0;
  }
}

void consumer_handler(int sig){
  if(sig = SIGUSR2){
    WAKE_CONSUMER = 0;
  }
}

int push_to_buf(int in_Num){
  int next = buffer->upper + 1;

  int ptr = buffer->upper
  printf("Upper pointer is: %u\n", ptr);

  if(next >= sizeof(buffer->buffer)){
    next = 0;
  }
  if(next == buffer->lower){
    printf("Buffer is full\n");
    return -1;
  }


    buffer->buffer[next] = in_Num;
    printf("%d Written to the buffer at  [%d]\n", in_Num, next);
    buffer->upper = next;
    buffer->count ++;
    return 0;

}

int pop_from_Buf(int *out_Num){
  int next;

  if (buffer->upper == buffer->lower){
    printf("Buffer is empty, nothing to read\n");
    return -1;
  }
  next = buffer->lower +1;
  if(next > sizeof(buffer->buffer)){
    next = 0;
  }

  *out_Num = buffer->buffer[buffer->lower];
  buffer->lower = next;
  return 0;
}

int producer(int ppid){
  while(STOP_PROD == 0){
    for (int i = 0; i < 200; i++){
      int foo = push_to_buf(i);
      if(foo == -1){
        STOP_PROD = 1;
      }else{
        kill(ppid, SIGUSR2);
      }
    }
  }
  return 0;
}

int consumer(int cpid, int *out_Num){
  while (WAKE_CONSUMER == 1) {
    sleep(1);
    }
    if(pop_from_Buf(*out_Num) == -1){
      WAKE_CONSUMER = 1;
      kill(cpid, SIGUSR2);
    }else{
      //printf("The current tail is: %d\n", *(int *) *buffer->lower);
      pop_from_Buf(*out_Num);
  }
}

int main(int argc, char const *argv[]) {

  buffer = (struct CIRCULAR_BUFFER*)mmap(0,sizeof(buffer), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1,0);

  buffer->count = 0;
  buffer->lower = 0;
  buffer->upper = 0;

  signal(SIGUSR1, prod_handler);
  signal(SIGUSR2, consumer_handler);

  pid_t pid = fork();

  if(pid == -1){
    printf("Can't fork, error%d\n", errno);
    exit(EXIT_FAILURE);
  }
  if(pid == 0){
    pid_t childPid = getppid();
    producer(pid);
  }
    pid_t parentPid = getpid();
    consumer(pid, buffer->lower);
    wait(NULL);

  return 0;
}
