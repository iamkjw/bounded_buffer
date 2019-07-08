#include <stdio.h>
#include <stdlib.h>
#include "stats.h"

typedef struct {
  int facNum; //factory number
  int made; //number of candies the factory reported making
  int eaten; //number of candies kids consumed
  float minDelay; //min time between production and consumtion (milliseconds)
  float totalDelay; //total time of all candy production/consumtion (milliseconds)
  float maxDelay; //max time between production and consumtion (milliseconds)
} factory_t;

void stats_init(int num_producers){
  //create array of candy factory structs
  factory_t *factories = malloc(num_producers * sizeof *factories);

  //initialize each candy factory
  for (int i = 0; i < num_producers; i++){
    factories[i].facNum = i;
    factories[i].made = 0;
    factories[i].eaten = 0;
    factories[i].minDelay = 0;
    factories[i].totalDelay = 0;
    factories[i].maxDelay = 0;
  }
}

//frees up any memory taken by the factories array
void stats_cleanup(void){
  free(factories);
}

//increments made counter everytime a candy is produced at a factory
void stats_record_produced(int factory_number){
  factories[factory_number].made++;
}

//increments eaten counter for every candy eaten by t_kids
//also keeps track of the min/max/total time taken between production and consumtion
void stats_record_consumed(int factory_number, double delay_in_ms){
  factories[factory_number].eaten++;
  //if min delay set as new min
  if (factories[factory_number].minDelay > delay_in_ms){
    factories[factory_number].minDelay = delay_in_ms;
  }
  //if max delay set as new max
  if (factories[factory_number].maxDelay < delay_in_ms){
    factories[factory_number].maxDelay = delay_in_ms;
  }
  //adding delay to the total delay
  factories[factory_number].totalDelay = factories[factory_number].totalDelay + delay_in_ms;
}

void stats_display(void){
  
}
