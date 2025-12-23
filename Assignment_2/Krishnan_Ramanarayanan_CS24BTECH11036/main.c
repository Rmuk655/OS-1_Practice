// Command to run it gcc -Wall -o ass2 main.c buff.c
// ./ass2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <string.h>

#include "buff.h"

#define BUFFER_SIZE 100
#define SEM_MUTEX 0
#define SEM_EMPTY 1
#define SEM_FULL 2

// int no_of_times;

// struct sem_buff
// {
//     unsigned short sem_num;
//     short sem_op;
//     short sem_flg;
// };

// union sema_union
// {
//     int val;
//     struct semid_ds *buf;
//     unsigned short *array;
// };

int create_shared_memory(struct shared_buffer **buff)
{
    int shmid = shmget(IPC_PRIVATE, sizeof(struct shared_buffer), 0666 | IPC_CREAT);
    *buff = (struct shared_buffer *)shmat(shmid, NULL, 0);
    return shmid;
}

void detach_shared_memory(struct shared_buffer *buff)
{
    shmdt(buff);
}

void destroy_shared_memory(int shmid)
{
    shmctl(shmid, IPC_RMID, NULL);
}

int create_semaphores()
{
    int sem_id = semget(IPC_PRIVATE, 3, 0666 | IPC_CREAT);
    return sem_id;
}

void init_semaphores(int semid)
{
    semctl(semid, SEM_MUTEX, SETVAL, 1);
    semctl(semid, SEM_EMPTY, SETVAL, BUFFER_SIZE);
    semctl(semid, SEM_FULL, SETVAL, 0);
}

void destroy_semaphores(int semid)
{
    semctl(semid, 0, IPC_RMID, 0);
}

void P(int semid, int semnum)
{
    struct sembuf op = {semnum, -1, 0};
    semop(semid, &op, 1);
}

void V(int semid, int semnum)
{
    struct sembuf op = {semnum, 1, 0};
    semop(semid, &op, 1);
}

void producer(int semid, struct shared_buffer *buff, int id, int no_of_times)
{
    for (int i = 0; i < no_of_times; i++)
    {
        int num = rand() % 10000;

        P(semid, SEM_EMPTY);
        P(semid, SEM_MUTEX);

        insert_item(buff, num);
        printf("Producer inserted %d into the buffer\n", num);

        V(semid, SEM_FULL);
        V(semid, SEM_MUTEX);

        sleep(1);
        // shmdt(buff);
        // _exit(0);
    }
    shmdt(buff);
    _exit(0);
}

void consumer(int semid, struct shared_buffer *buff, int no_of_times)
{
    for (int i = 0; i < no_of_times; i++)
    {
        P(semid, SEM_FULL);
        P(semid, SEM_MUTEX);

        int num = remove_item(buff);
        printf("Consumer removed %d from the buffer\n", num);

        V(semid, SEM_MUTEX);
        V(semid, SEM_EMPTY);

        sleep(1);
        // shmdt(buff);
        // _exit(0);
    }
    shmdt(buff);
    _exit(0);
}

int main(int times, char *vec[])
{
    if (times < 2)
    {
        exit(0);
    }
    int no_of_times = atoi(vec[1]);
    struct shared_buffer *buff;
    int shmid = create_shared_memory(&buff);
    init_buffer(buff);

    int semid = create_semaphores();
    init_semaphores(semid);

    // scanf("%d", &no_of_times);

    int pid1 = fork();
    if (pid1 == 0)
    {
        srand(getpid());
        producer(semid, buff, 1, no_of_times);
    }
    
    int pid2 = fork();
    if (pid2 == 0)
    {
        srand(getpid());
        producer(semid, buff, 2, no_of_times);
    }
    
    int pid3 = fork();
    if (pid3 == 0)
    {
        srand(getpid());
        consumer(semid, buff, no_of_times);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    shmdt(buff);
    destroy_shared_memory(shmid);
    destroy_semaphores(semid);
}