#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <buffer.h>
typedef int buffer item;
#define BUFFER SIZE 5;
void *producer(void *param) {
 buffer item item;

 while (true) {
 /* sleep for a random period of time */
 sleep(...);
 /* generate a random number */
 item = rand();
 if (insert item(item))
 fprintf("report error condition");
 else
 printf("producer produced %d\n",item);
 }
}
void *consumer(void *param) {
 buffer item item;

 while (true) {
 /* sleep for a random period of time */
 sleep(...);
 if (remove item(&item))
 fprintf("report error condition");
 else
 printf("consumer consumed %d\n",item);
 }
}
