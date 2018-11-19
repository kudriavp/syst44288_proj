#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"
typedef int buffer_item;
#define BUFFER_SIZE 5;

buffer_item buffer[BUFFER_SIZE];

sem_t s_empty;
sem_t s_full;
pthread_mutex_t mutex;

int insert_ptr=0, remove_ptr=0;

int insertItem(buffer_item item)
/*insert item into the buffer*/
{
    int x=0;
    sem_wait(&s_empty);
}


