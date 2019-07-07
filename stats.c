#include <stdio.h>
#include <stdlib.h>
#include "stats.h"

typedef struct {
  int facNum; //factory number
  int made; //number of candies the factory reported making
  int eaten; //number of candies kids consumed
  float minDelay; //min time between production and consumtion (milliseconds)
  float avgDelay; //avg time between production and consumtion (milliseconds)
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
  }
}

void stats_cleanup(void);
void stats_record_produced(int factory_number);
void stats_record_consumed(int factory_number, double delay_in_ms);
void stats_display(void);
