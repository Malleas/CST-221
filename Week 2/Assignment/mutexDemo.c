#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int balance = 0;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//sem_t mutex;
void* depositFunds(void *arg){
  int input = *(int *) arg;
  for (int i = 0; i < 1000000; i++){
    //pthread_mutex_lock(&mutex);
    //sem_wait(&mutex);
    balance += input;
    //sem_post(&mutex);
    //pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}


int main(int argc, char const *argv[]) {

  //sem_init(&mutex, 0, 1);
  pthread_t id1;
  int depositAmt = 1;
  pthread_create(&id1, NULL, depositFunds, &depositAmt);

  pthread_t id2;
  pthread_create(&id2, NULL, depositFunds, &depositAmt);

  pthread_join(id1, NULL);
  pthread_join(id2, NULL);

//  sem_destroy(&mutex);

  printf("Your current balance is: $%d\n", balance);

  return 0;
}
