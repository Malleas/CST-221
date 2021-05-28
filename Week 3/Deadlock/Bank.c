//
// Created by matt on 5/28/21.
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>
#include <sys/wait.h>

//globals
int balance = 10000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t *semaphore;
int misses;
//thread function to add or sub $$ from balance
void* bank(void *arg){
   // convert inbound amt to an int
    int amt = *(int *) arg;
    //check status of trywait
    int status = sem_trywait(semaphore);
    //int status = pthread_mutex_trylock(&mutex);
    //if trywait status (or trylock) == 0 proceed, else increase misses for un settled funds
    if(status == 0) {

        printf("In bank func for amt: %d\n", amt);
        printf("My amount is: %d\n", amt);
        //if amt + balance is greater than 0, issue loan (withdrawal funds)
        if (amt < 0 && (balance + amt) > 0) {
            balance = balance + amt;
            printf("Loan issued in the amount of %d.  Remaining balance is: %d\n", abs(amt), balance);
            sem_post(semaphore);
            //pthread_mutex_unlock(&mutex);

        } else if ((balance + amt) < 0) {
            //else if the amt + balance would be a negative number, no withdrawal is made
            printf("Loan request denied, current balance of: %d is insufficient to cover requested loan amount of: %d\n",balance, abs(amt));
            sem_post(semaphore);
            //pthread_mutex_unlock(&mutex);
        } else {
            //else, funds must be positive, deposit is made
            balance = balance + amt;
            printf("Deposit of %d made to and balance is now: %d\n", amt, balance);
            sem_post(semaphore);
            //pthread_mutex_unlock(&mutex);
        }
    } else{
        //catch any transactions from threads that fail to get an unlock status and add them to misses to see if any funds went unsettled.
        printf("Unable to update for amt %d. Balance is currently locked with a status of: %d\n", amt, status);
        misses = misses + amt;
    }
       sem_post(semaphore);
    //pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

int main(int argc, char const *argv[]) {
    //create a semaphore in a shared memory location.  This was borrowed from Activity 3 Semaphores
    semaphore = (sem_t *) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sem_init(semaphore, 1, 1) != 0) {
        printf("Failed to create semaphore\n");
        exit(EXIT_FAILURE);
    }
    //set a min and a max for rand
    int max = 10000;
    int min = -10000;
    pthread_t tid;
    //create a number of threads to run.  Update the value in the middle to increase or decrease
    for (int i = 0; i <= 10; ++i) {
        //create a random number that is either + or - to mimic withdrawals or deposits.
        int amt = (rand()%(max+1-min))+min;
        printf("Amt for index %d is: %d\n", i, amt);
        pthread_create(&tid, NULL, bank, &amt);
    }

    pthread_join(tid, NULL);
    printf("Total amt unable to be deposited due to lock status: %d\n", misses);
    sem_destroy(semaphore);
}




