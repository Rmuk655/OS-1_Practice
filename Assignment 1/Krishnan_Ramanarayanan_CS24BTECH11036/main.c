#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "Queue.h"

#define QUEUE_SIZE 2

pthread_mutex_t makeAtomic = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;

// To run the program, the command is gcc main.c Queue.c -o main in VS-Code and
// gcc -Wall main.c Queue.c -o main -lpthread in WSL.

int times;
void *thread1(void *);
void *thread2(void *);
void print_queue(struct Queue *queue)
{
    if (isEmptyQ(queue))
    {
        printf("\n");
        return;
    }
    if (queue->tail > queue->head)
    {
        for (int i = queue->head; i < queue->tail; i++)
        {
            printf("%d ", queue->arr[i]);
        }
    }
    else
    {
        for (int i = queue->head; i < queue->length; i++)
        {
            printf("%d ", queue->arr[i]);
        }
        for (int i = 0; i < queue->tail; i++)
        {
            printf("%d ", queue->arr[i]);
        }
    }
    printf("\n");
}

void Producer(void *arg)
{
    struct Queue *queue = (struct Queue *)arg;
    pthread_mutex_lock(&makeAtomic);
    if (isFullQ(queue))
    {
        printf("[Producer] Queue full, waiting...\n");
        pthread_cond_wait(&notFull, &makeAtomic);
    }
    int num = rand();
    addQ(queue, num);
    printf("[Producer] %d inserted into the queue.\n", num);
    print_queue(queue);
    pthread_cond_signal(&notEmpty);
    pthread_mutex_unlock(&makeAtomic);
}

void Consumer(void *arg)
{
    struct Queue *queue = (struct Queue *)arg;
    pthread_mutex_lock(&makeAtomic);
    if (isEmptyQ(queue))
    {
        printf("[Consumer] Queue empty. Waiting...\n");
        pthread_cond_wait(&notEmpty, &makeAtomic);
    }
    int del_num = deleteQ(queue);
    printf("[Consumer] %d deleted from the queue.\n", del_num);
    print_queue(queue);
    pthread_cond_signal(&notFull);
    pthread_mutex_unlock(&makeAtomic);
}

int main()
{
    scanf("%d", &times);
    srand(time(NULL));
    struct Queue *queue = initQ(QUEUE_SIZE);
    pthread_t producer_thread, consumer_thread;

    pthread_create(&producer_thread, NULL, thread1, queue);
    pthread_create(&consumer_thread, NULL, thread2, queue);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
}

void *thread1(void *vp)
{
    struct Queue *queue = (struct Queue *)vp;
    int i;
    for (i = 1; i <= times; i++)
    {
        Producer(queue);
    }
    return NULL;
}

void *thread2(void *vp)
{
    struct Queue *queue = (struct Queue *)vp;
    int i;
    for (i = 1; i <= times; i++)
    {
        Consumer(queue);
    }
    return NULL;
}