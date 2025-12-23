#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "Queue.h"
#include <unistd.h>

struct Queue *initQ(int max_length)
{
    struct Queue *queue = (struct Queue *)calloc(1, sizeof(struct Queue));
    queue->arr = (int *)calloc(max_length, sizeof(int));
    queue->length = max_length;
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
    return queue;
}

bool isEmptyQ(struct Queue *queue)
{
    return queue->count == 0;
}

bool isFullQ(struct Queue *queue)
{
    return queue->count == queue->length;
}

void addQ(struct Queue *queue, int ele)
{
    // sleep(1);
    // Basic check to see if the queue is full or not, as we cannot add elements to a full Queue.
    if (isFullQ(queue))
    {
        printf("Overflow Error! Cannot insert element into a full Queue.\n");
        return;
    }
    queue->arr[queue->tail] = ele;
    queue->tail = (queue->tail + 1) % queue->length;
    queue->count++;
}

int deleteQ(struct Queue *queue)
{
    sleep(1);
    // Basic check to see if the queue is empty or not as we cannot remove elements from an empty queue.
    if (isEmptyQ(queue))
    {
        printf("Underflow Error! Cannot delete element from an empty Queue.");
        return INT_MIN; // By returning INT_MIN, the function tells us there was an error dequeuing the element.
    }
    // Finding the element to dequeue and storing it in the variable x.
    int x = queue->arr[queue->head];
    // Set it to 0;
    queue->arr[queue->head] = 0;
    // We now set the head value of queue
    queue->head = (queue->head + 1) % queue->length;
    queue->count--;
    return x;
}
int frontQ(struct Queue *queue)
{
    if (isEmptyQ(queue))
    {
        printf("Queue is empty. Hence, cannot return the front element!\n");
        return INT_MIN;
    }
    return queue->arr[queue->head];
}