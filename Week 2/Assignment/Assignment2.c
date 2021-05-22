#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

 void doWork(int sleepNum, int pid){
   for (int i = 0; i<10; i++){
     printf("The current index is: %d for pid: %d\n", i, pid);
     sleep(sleepNum);
   }
 }

 int main(){
   printf("Starting Process\n");

   pid_t pid = fork();

  if (pid == 0){
    printf("Entering Child process.\n");
    doWork(1, pid);
    printf("Exiting Child\n");
    exit(0);
  }

  printf("Entering Parent process with pid: %d\n", pid);
  doWork(2, pid);
  printf("Exiting Parent\n");
  exit(0);
 }
