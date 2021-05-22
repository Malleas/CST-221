#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char const *argv[]) {
  int a = 0;
  int pid = getpid();
  printf("Start a = %d and it's pid = %d\n", a, pid);
   fork();
    pid = getpid();
  if(fork() == 0){
    //int pid = getpid();
    a = 2;
    printf("First if: %d and it's pid is: %d\n", a, pid);
  }
  if (fork() == 0){
    //int pid = getpid();
    a = 3;
    printf("Second if: %d and it's pid is: %d\n", a, pid);

  }
  fork();
 pid = getpid();
  printf("End a = %d and it's pid = %d\n", a, pid);


  return 0;
}
