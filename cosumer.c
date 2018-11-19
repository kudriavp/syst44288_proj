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

/*int insertItem(buffer_item item)
/*insert item into the buffer*/
/*{
    int x=0;
    sem_wait(&s_empty);
}*/

int wait(int s)
{
    return (--s);
}
int signal(int s)
{
    return(++s);
}
void producer(void)
{
    unsigned int run_cnt;
    unsigned int exec_period_usecs;
    struct timeval ts;

    exec_period_usecs = 1000000; /in micro-seconds/

    printf("Thread 1 started. Execution period = %d uSecs\n",\exec_period_usecs);
    run_cnt = 0;

    while(1)
    {
        usleep(exec_period_usecs);
        mutex=wait(mutex);
        full=signal(full);
        empty=wait(empty);
        x++;
        printf("\nT1. Producer produces the item %d",x);
        mutex=signal(mutex);
    }
}
