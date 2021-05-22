#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
  fork();
  fork();
  printf("Hello\n");
  exit(0);
}
