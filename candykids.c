#include "bbuff.c"
#include "stats.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct  {
    int factory_number;
    double time_stamp_in_ms;
} candy_t;

_Bool stop_thread = false;

void *candy_factory_function(void* arg) {
  //extract arg to obtain factory number
  int *num = (int*)arg;
  //runs until main() signals to exit
  while(!stop_thread){
    //wait random time between 0-3 inclusive seconds
    int wait_time = rand()%3;
    printf("\tFactory %d ships candy & waits &ds", num, wait_time);
    //allocate new candy item and populate its fields
    candy_t *new_candy = malloc(sizeof(candy_t));
    new_candy->factory_number = num;
    new_candy->time_stamp_in_ms =

    //adding candy to bounded buffer
    bbuff_blocking_insert(new_candy);

    sleep(wait_time);
  }
  printf("Candy-factory %d done", num);
}

void *kid_function() {
  //loop forever
  while(1){
    //sleep random time between 0-1 inclusive seconds
    int wait_time = rand()%1;
    //extract candy from bounded buffer
    printf("%d", bbuff_blocking_extract();

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
    for(int i = 0;i < number_of_factories; i++){

    }


    // 4.  Launch kid threads
    for(int j = 0;j < number_of_kids; j++){

    }
    // 5.  Wait for requested time
    // 6.  Stop candy-factory threads
    // 7.  Wait until no more candy
    // 8.  Stop kid threads
    // 9.  Print statistics
    // 10. Cleanup any allocated memory
}
