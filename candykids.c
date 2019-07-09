#include "bbuff.h"
#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#include <semaphore.h>
#include <time.h>

typedef struct  {
    int factory_number;
    double time_stamp_in_ms;
} candy_t;

double current_time_in_ms(void)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

_Bool stop_thread = false;

void *candy_factory_function(void* arg) {
  //extract arg to obtain factory number
  int *num = (int*)arg;
  //runs until main() signals to exit
  while(!stop_thread){
    //wait random time between 0-3 inclusive seconds
    int wait_time = rand()%3;
    printf("Factory %d ships candy & waits %ds\n", *num, wait_time);
    //allocate new candy item and populate its fields
    candy_t *new_candy = malloc(sizeof(candy_t));
    new_candy->factory_number = *num;
    new_candy->time_stamp_in_ms = current_time_in_ms();

    //adding candy to bounded buffer
    bbuff_blocking_insert(new_candy);
    stats_record_produced(*num);

    sleep(wait_time);
  }
  printf("Candy-factory %d done\n", *num);
  return NULL;
}

void *kid_function() {
  //loop forever
  while(1){
    //sleep random time between 0-1 inclusive seconds
    int wait_time = rand()%1;
    //extract candy from bounded buffer
    candy_t* consumed = bbuff_blocking_extract();
    stats_record_consumed(consumed->factory_number, current_time_in_ms() - consumed->time_stamp_in_ms);

    sleep(wait_time);
  }
}

int main(int argc, char** argv) {
    // 1.  Extract arguments
    int number_of_factories = atoi(argv[1]);
    int number_of_kids = atoi(argv[2]);
    int number_of_seconds = atoi(argv[3]);

    //Ensures that all arguments are greater than 0
    if(number_of_factories <= 0 || number_of_kids <= 0 || number_of_seconds <= 0){
      printf("Error: arguments are not greater than 0\n");
      exit(1);
    }

    // 2.  Initialize modules
    stats_init(number_of_factories);
    bbuff_init();
    //for random number generator
    srand(time(NULL));

    // 3.  Launch candy-factory threads
    //create array holding the factory threads
    pthread_t factory_thread_array[number_of_factories];
    //create array holding factory numbers to join on later
    int factory_id_array[number_of_factories];

    for(int i = 0; i < number_of_factories; i++){
      factory_id_array[i] = i;
      pthread_create(&factory_thread_array[i],
                      //attr to be passed to create function
                      0,
                      //function to be executed
                      candy_factory_function,
                      //arg to be passed into function
                      &factory_id_array[i]);
    }

    // 4.  Launch kid threads
    //create array holding the kid threads
    pthread_t kid_thread_array[number_of_kids];

    for(int j = 0; j < number_of_kids; j++){
      pthread_create(&kid_thread_array[j],
                      //attr to be passed to create function
                      0,
                      //function to be executed
                      kid_function,
                      //arg to be passed into function
                      NULL);
    }

    // 5.  Wait for requested time
    for(int k = 0; k < number_of_seconds; k++){
      sleep(1);
      printf("Time %ds\n", k+1);
    }

    // 6.  Stop candy-factory threads
    stop_thread = true;
    for (int l = 0; l < number_of_factories; l++){
      pthread_join (factory_thread_array[l], NULL);
    }

    // 7.  Wait until no more candy
    while (bbuff_is_empty() == false){
      printf("Waiting for all candy to be consumed\n");
      sleep(1);
    }

    // 8.  Stop kid threads
    for (int m = 0; m < number_of_kids; m++){
      pthread_cancel (kid_thread_array[m]);
      pthread_join (kid_thread_array[m], NULL);
    }

    // 9.  Print statistics
    stats_display();

    // 10. Cleanup any allocated memory
    stats_cleanup();
    return 0;
}
