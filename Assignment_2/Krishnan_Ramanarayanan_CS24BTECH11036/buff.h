#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 100

struct shared_buffer
{
    int buffer[BUFFER_SIZE];
    int in;
    int out;
};

void init_buffer(struct shared_buffer *buff);
void insert_item(struct shared_buffer *buff, int ele);
int remove_item(struct shared_buffer *buff);