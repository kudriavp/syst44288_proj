#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
typedef int buffer_item;
#define BUFFER_SIZE 5

buffer_item buffer[BUFFER_SIZE];
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int insertval = 0, removeval = 0;

void *producer(void *params);
void *consumer(void *params);

int insert_item(buffer_item item)
{
    int ret_n;
    sem_wait(&empty);
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
    pthread_mutex_unlock(&mutex);
    sem_post(&full);

    return ret_n;
}

int remove_item(buffer_item *item)
{
    int ret_n = 0;
    sem_wait(&full);

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
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    return ret_n;
}

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

int main(int argc, char *argv[])
{
    int runtime, prodtd, constd;
    int i, j;

    if(argc != 4)
    {
        fprintf(stderr, "Please input as args: Run time, Producer threads, Consumer threads\n");
        return -1;
    }

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
