#include <stdio.h>
#include <stdlib.h>
#include "stats.h"

typedef struct {
  int made; //number of candies the factory reported making
  int eaten; //number of candies kids consumed
  float minDelay; //min time between production and consumtion (milliseconds)
  float totalDelay; //total time of all candy production/consumtion (milliseconds)
  float maxDelay; //max time between production and consumtion (milliseconds)
} factory_t;

factory_t *factories;

void stats_init(int num_producers){
  //create array of candy factory structs
  factories = malloc(num_producers * sizeof *factories);

  //initialize each candy factory
  for (int i = 0; i < num_producers; i++){
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
  printf("Statistics:\n");
  printf("%8s %10s %10s %15s %15s %15s\n", "Factory#", "#Made", "#Eaten", "Min Delay[ms]", "Avg Delay[ms]", "Max Delay[ms]");

  //finding number of factories in factories array
  int len = sizeof(factories)/sizeof(factory_t);

  for (int i = 0; i < len; i++){
    //If the #Made and #Eaten columns don’t match, print an error
    if(factories[i].made != factories[i].eaten){
      printf("ERROR: Mismatch between number made and eaten");
      continue;
    }
    printf("%8d %10.5d %10.5d %15.5f %15.5f %15.5f\n",
          i,
          factories[i].made,
          factories[i].eaten,
          factories[i].minDelay,
          //find avg by dividing totaldelay by number eaten
          factories[i].totalDelay/factories[i].eaten,
          factories[i].maxDelay);
  }
}
