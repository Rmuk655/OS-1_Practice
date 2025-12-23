#include <stdio.h>
#include <stdlib.h>
#include "buff.h"

#define BUFFER_SIZE 100

void init_buffer(struct shared_buffer *buff)
{
    buff->in = 0;
    buff->out = 0;
}

void insert_item(struct shared_buffer *buff, int ele)
{
    buff->buffer[buff->in] = ele;
    buff->in = (buff->in + 1) % BUFFER_SIZE;
}

int remove_item(struct shared_buffer *buff)
{
    int item = buff->buffer[buff->out];
    buff->out = (buff->out + 1) % BUFFER_SIZE;
    return item;
}