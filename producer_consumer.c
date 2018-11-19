#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
typedef int buffer_item;
#define BUFFER_SIZE 5

#define TRUE 1

buffer_item buffer[BUFFER_SIZE];

pthread_t thread1;
pthread_t thread2;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int insertPointer = 0, removePointer = 0;

void producer(void);
void consumer(void);

int insert_item(buffer_item item)
{
    int ret_n = 0;   
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    if (insertPointer < BUFFER_SIZE) 
    {
        buffer[insertPointer++] = item;
        insertPointer = insertPointer % 5;
    } 
    else 
    {
        ret_n = -1;
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&full);

    return ret_n;
}

int remove_item(buffer_item *item)
{
    int ret_n;
    sem_wait(&full);

    pthread_mutex_lock(&mutex);
    if (insertPointer > 0) 
    {
        *item = buffer[removePointer];
        buffer[removePointer++] = -1;
        removePointer = removePointer % 5;
    } 
    else 
    {
        ret_n = -1;
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    return ret_n;
}


int main(int argc, char *argv[])
{
    int producerThreads, consumerThreads;

    /* Initialize the synchronization tools */
    printf("%d\n",pthread_mutex_init(&mutex, NULL));
    printf("%d\n",sem_init(&empty, 0, 2));
    printf("%d\n",sem_init(&full, 0, 0));
    srand(time(0));

    pthread_create(&thread1, NULL, producer, NULL);
    pthread_create(&thread2, NULL, consumer, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}

void producer(void)
{
    buffer_item random;
    int r;

    while(TRUE)
    {
        r = rand() % 5;
        sleep(r);
        random = rand();

        if(insert_item(random))
            fprintf(stderr, "Error");

        printf("Producer produced %d \n", random);

    }

}

void consumer(void)
{
    buffer_item random;
    int r;

    while(TRUE)
    {
        r = rand() % 5;
        sleep(r);

        if(remove_item(&random))
            fprintf(stderr, "Error Consuming\n");
        else
            printf("Consumer consumed %d \n", random);
    }
}