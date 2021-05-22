#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int currentStock = 1000000;
int counter =- 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* checkInventory(){
  int stock_increase = 100000;
    if(currentStock <= 250000){
      pthread_mutex_lock(&mutex);
      currentStock = currentStock + stock_increase;
      pthread_mutex_unlock(&mutex);
      printf("Stock increased to avoid out of stock scenario, wty added: %d\n", stock_increase);
    }else{
      printf("Current stock is at: %d, no need to restock yet.\n", currentStock);
    }
}

void* updateInventory(void *arg){
  int reduce_inv_by = *(int *) arg;
  for (int i = 0; i < 1000000; i++){
    if(counter % 5 == 0){
      checkInventory();
    }
    pthread_mutex_lock(&mutex);
    currentStock = currentStock - reduce_inv_by;
    pthread_mutex_unlock(&mutex);
    counter = counter + 1;
  }
}


int main(int argc, char const *argv[]) {

  pthread_t purchaseId;
  int purchaseQty = 10;
  pthread_create(&purchaseId, NULL, updateInventory, &purchaseQty);

  pthread_t replenishId;
  int repleinshQty = 10;
      pthread_create(&replenishId, NULL, updateInventory, &repleinshQty);


  pthread_join(purchaseId, NULL);
  pthread_join(replenishId, NULL);

  printf("the ending stock level is: %d\n", currentStock);
  return 0;
}
