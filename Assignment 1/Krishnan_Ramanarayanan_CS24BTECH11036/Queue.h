#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdbool.h>

struct Queue
{
    int *arr;
    int length;
    int head;
    int tail;
    int count;
};

struct Queue *initQ(int max_length);
void addQ(struct Queue *queue, int ele);
int deleteQ(struct Queue *queue);
int frontQ(struct Queue *queue);
bool isEmptyQ(struct Queue *queue);
bool isFullQ(struct Queue *queue);
#endif