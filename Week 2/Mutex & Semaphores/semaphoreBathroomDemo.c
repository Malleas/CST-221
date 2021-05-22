#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

static int numberOfTickets = 15;
static int salesPerson = 15;
sem_t ticketLock;

static void* sellTicket(void *arg){
  int spid = * (int *) arg;
  int ticketsSold = 0;
  bool isDone = false;


  while (!isDone){
    sleep(rand()%(2-0+1)+1);
    sem_wait(&ticketLock);
    if(numberOfTickets == 0){
      isDone = true;
      sem_post(&ticketLock);

    }else{

      numberOfTickets = numberOfTickets - 1;
      ticketsSold = ticketsSold + 1;
     sem_post(&ticketLock);
      printf("%d Sold a ticket\n", spid);
    }

  }
  printf("No Tickets left to sell, Sales Person %d sold a total of %d tickets\n", spid, ticketsSold);
}


int main(int argc, char const *argv[]) {
  sem_init(&ticketLock, 0, 1);
  pthread_t tid[salesPerson];
  for (int i = 0; i < salesPerson; i++){
    pthread_create(&tid[i], NULL, sellTicket, &i);
    printf("Patron %d is using the bathroom, ID: %d\n", i, *(int *) tid[i]);
  }


  for (int i = 0; i < salesPerson; i++){
    pthread_join(tid[i], NULL);
  }
  sem_destroy(&ticketLock);


  return 0;
}
