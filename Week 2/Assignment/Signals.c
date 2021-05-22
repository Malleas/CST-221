#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static int WAKEUP = 0;

void handler(int sig){
  if(sig = SIGUSR1){
    WAKEUP = 1;
  }
}

int main(int argc, char const *argv[]) {

  signal(SIGUSR1, handler);

  pid_t pid = fork();

  if(pid == 0){

    while (WAKEUP == 0) {
      sleep(1);
    }
    for (int i = 0; i < 20; i++){
      printf("The current child index is: %d\n", i);
      sleep(1);
    }
    exit(0);
  }

  for (int i = 0; i < 30; i++){
    printf("The current Parent index is: %d\n", i);
    sleep(1);
    if (i+1 > 4){
      kill(pid, SIGUSR1);
    }
  }
  wait(NULL);
    exit(0);

  return 1;
}
