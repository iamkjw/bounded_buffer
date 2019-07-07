#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include "bbuff.h"

void* buffer[BUFFER_SIZE];

//empty counts empty slots in the buffer
sem_t empty;
//data counts number of data items in the buffer
sem_t data;
//accounts for mutual exclusion during updates (lock)
sem_t mutex;

//indexes of the front and rear of buffer
int next_in;
int next_out;

void bbuff_init(void){
  //initialize empty to buffer size
  sem_init (&empty, 0, BUFFER_SIZE);
  //initialize data to 0
  sem_init (&data, 0, 0);
  //initialize mutex to 1
  sem_init (&mutex, 0, 1);

  next_in = 0;
  next_out = 0;
}

void bbuff_blocking_insert(void* item){
  //decrements empty, if buffer is full (ie. empty = 0) waits until item is removed
  sem_wait (&empty);
  //checks if other threads are in the critical section
  sem_wait (&mutex);

  //critical section: add item to buffer in next_in index
  buffer[next_in] = item;
  next_in = (next_in + 1) % BUFFER_SIZE;

  //releases the mutex
  sem_post(&mutex);
  //increments data after inserting item
  sem_post(&data);
}

void* bbuff_blocking_extract(void){
  //decrements data, if buffer is empty (ie. data = 0) waits until item is added
  sem_wait (&data);
  //checks if other threads are in the critical section
  sem_wait (&mutex);

  //critical section: remove item from buffer in next_out index
  void* consumable = buffer[next_out];
  buffer[next_out] = NULL;
  next_out = (next_out + 1) % BUFFER_SIZE;

  //releases the mutex
  sem_post(&mutex);
  //increments data after removing item
  sem_post(&empty);

  return consumable;
}

_Bool bbuff_is_empty(void){
  //aquire mutex
  sem_wait (&mutex);

  //obtain semaphore value
  int isempty;
  sem_getvalue (&data, isempty);

  //release mutex
  sem_post (&mutex);

  if (isempty == 0){
    return true;
  }
  else {
    return false;
  }
}
