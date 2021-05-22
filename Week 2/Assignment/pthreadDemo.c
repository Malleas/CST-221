#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* thread1 (){
  for (int i = 0; i < 10; i++){
    printf("Printing from thread1 with index: %d\n", i);
    sleep(1);
  }
  return NULL;
}
void* thread2 (){
  for (int i = 0; i < 10; i++){
    printf("Printing from thread2 with index: %d\n", i);
    sleep(2);
  }
  return NULL;
}
int main(int argc, char const *argv[]) {
  pthread_t tid;

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  pthread_create(&tid, &attr, thread1, NULL);
  pthread_create(&tid, &attr, thread2, NULL);

  pthread_join(tid, NULL);
  pthread_join(tid, NULL);
  return 0;
}
