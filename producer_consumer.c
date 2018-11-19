#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
// The internal buffer defined by a typedef consisting of a fixed-size array
typedef int buffer_item;
#define BUFFER_SIZE 5

buffer_item buffer[BUFFER_SIZE];
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int insertval = 0, removeval = 0;

void *producer(void *params);
void *consumer(void *params);

/* The following code is the insert_item function which will manipulate the buffer and gets called by the producer and consumer
   processes, respectively. */
int insert_item(buffer_item item)
{
    int ret_n;
    /* Puts semaphore 'empty' into a waiting state, waiting to see if the following if statement will return successful(0),
       or or an error condition(-1). */
    sem_wait(&empty);
    // Locks the mutual-exclusion object 'mutex'
    pthread_mutex_lock(&mutex);
    if (insertval < BUFFER_SIZE)
    {
        ret_n = 0;
        buffer[insertval] = item;
        insertval++;
        insertval = insertval % BUFFER_SIZE;
    }
    else
    {
        ret_n = -1;
        fprintf(stderr, "===== ERROR: Buffer Full ===== \n");
    }
    // Unlocks the mutual-exclusion object 'mutex'
    pthread_mutex_unlock(&mutex);
    // Increment the value of semaphore 'full' and wakes up the blocked process waiting on this one
    sem_post(&full);
    // Returns the result if adding the object to the buffer was successful or not
    return ret_n;
}

/* The following code is the remove_item function which will manipulate the buffer and gets called by the producer and consumer
   processes, respectively. */
int remove_item(buffer_item *item)
{
    int ret_n = 0;
    /* Puts semaphore 'full' into a waiting state, waiting to see if the following if statement will return successful(0),
       or or an error condition(-1). */
    sem_wait(&full);
    // Locks the mutual-exclusion object 'mutex'
    pthread_mutex_lock(&mutex);
    if (removeval < BUFFER_SIZE && removeval >= 0)
    {   
        ret_n = 0;
        *item = buffer[removeval];
        buffer[removeval] = 0;
        removeval++;
        removeval = removeval % BUFFER_SIZE;
    } 
    else
    {
        ret_n = -1;
        fprintf(stderr, "===== ERROR: Buffer Empty ===== \n");
    }
    // Unlocks the mutual-exclusion object 'mutex'
    pthread_mutex_unlock(&mutex);
    // Increment the value of semaphore 'empty' and wakes up the blocked process waiting on this one
    sem_post(&empty);
    // Returns the result if removing the object from the buffer was successful or not
    return ret_n;
}

//The following code is the producer process
void *producer(void *param)
{
    buffer_item item;
    int rn;

    while(1)
    {
        rn = rand() % 4;
        sleep(rn);
        item = rand();

        if(insert_item(item))
        {
            fprintf(stderr, "Producer error\n");
        }
        else
        {
            printf("Producer produced %d\n", item);
        }
    }
}

// The following code is the consumer process
void *consumer(void *param)
{
    buffer_item item;
    int rn;

    while(1)
    {
        rn = rand() % 4;
        sleep(rn);

        if(remove_item(&item))
        {
            fprintf(stderr, "Consumer error\n");
        }
        else
        {
            printf("Consumer consumed %d \n", item);
        }
    }
}

/* The following code is the main function which will initialize the buffer and create the separate producer and consumer threads. After
   the producer and consumer threads are created, this main function will sleep for a period of time and, upon awakening, will terminate
   the application. The three parameters that are passed to this main function from the command line are:
   1. How long to sleep before terminating
   2. The number of producer threads
   3. The number of consumer threads */
int main(int argc, char *argv[])
{
    int runtime, prodtd, constd;
    int i, j;

    if(argc != 4)
    {
        fprintf(stderr, "Please input as args: Run time, Producer threads, Consumer threads\n");
        return -1;
    }

    // Acquire all of the appropriate command line arguments and initialize them into variables
    runtime = atoi(argv[1]);
    prodtd = atoi(argv[2]);
    constd = atoi(argv[3]);

    /* Initialize the synchronization tools */
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    srand(time(0));

    /* Create the producer and consumer threads */
    for(i = 0; i < prodtd; i++)
    {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, producer, NULL);
    }

    for(j = 0; j < constd; j++)
    {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, consumer, NULL);
    }

    /* Sleep for user specified time */
    sleep(runtime);
    return 0;
}
