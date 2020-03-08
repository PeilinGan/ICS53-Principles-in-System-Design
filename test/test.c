#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>           
#include <sys/stat.h>       


int main()
{
    sem_t  mutex;
    int rvalue = sem_init(&mutex, 0, 8);
    printf("slots %d \n", rvalue);
}