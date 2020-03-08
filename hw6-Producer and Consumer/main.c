#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>

int TIMEDELAY = 5 * 100000;

int p_count, c_count;
int item_num;
int p_current = 0;
int c_current = 0;
int delay;

typedef struct
{
    int *buf;
    int n;
    int front;
    int rear;
    sem_t mutex;
    sem_t slots;
    sem_t items;
} buf;

buf Buf;

void init_buf(buf *sp, int n)
{
    sp->buf = calloc(n, sizeof(int));
    // init buf with n items
    sp->n = n;
    // init front and rear to be zero
    sp->front = sp->rear = 0;
    // init semaphores mutex, slots and  # items
    sem_init(&sp->mutex, 0, 1);
    int rvalue = sem_init(&sp->slots, 0, 8);
    printf("slots %d \n",rvalue);
    sem_init(&sp->items, 0, 0);
}

void clean_buf(buf *sp)
{
    free(sp->buf);
}

// insert onto the rear of buf
void insert_buf(buf *sp, int item, int pid)
{
    sem_wait(&sp->slots);
    sem_wait(&sp->mutex);
    sp->buf[(sp->rear) % (sp->n)] = item;
    printf("rear: %d\n", (sp->rear) % (sp->n));
    sp->rear++;
    // if (delay == 0)
    //     usleep(TIMEDELAY);
    // printf("producer_%d produced item %d\n", pid, item);
    printf("slots: %d\n", sp->slots);
    sem_post(&sp->mutex);
    sem_post(&sp->items);
}

// remove and return the first item from buf
int remove_buf(buf *sp, int cid)
{

    int item;
    sem_wait(&sp->items);
    sem_wait(&sp->mutex);
    item = sp->buf[(sp->front) % (sp->n)];
    sp->front++;
    // if (delay)
    //     usleep(TIMEDELAY);
    // printf("consumer_%d consumed item %d\n", cid, item);
    sem_post(&sp->mutex);
    sem_post(&sp->slots);
    return item;
}

void Producer(int p_id)
{
    int producer_num = item_num;
    // || c_current != c_count
    while (p_current != p_count )
    {
    }
    printf("p_current: %d, p_count: %d \n", p_current, p_count);
    
    int i;
    for (i = 0; i < producer_num; i++)
    {
        // printf("pid: %d, producer_num: %d\n", p_id, producer_num);
        int item = p_id * producer_num + i;
        // printf("item: %d\n", item);
        insert_buf(&Buf, item, p_id);
    }
}

void Consumer(int c_id)
{
    int consumer_num = p_count * item_num / c_count;
    while (p_current != p_count || c_current != c_count)
    {
    }
    int i;
    for (i = 0; i < consumer_num; i++)
    {
        int item = c_id * consumer_num + i;
        remove_buf(&Buf, c_id);
    }
}

int main(int argc, char *argv[])
{
    p_count = atoi(argv[1]);
    c_count = atoi(argv[2]);
    item_num = atoi(argv[3]);
    delay = atoi(argv[4]);
    init_buf(&Buf, 8);
    pthread_t p_tid[p_count];
    pthread_t c_tid[c_count];

    int i, j;
    for (i = 0; i < p_count; i++)
    {
        p_current += 1;
        pthread_create(&p_tid[i], NULL, Producer, (int)i);
    }
    // for (j = 0; j < c_count; j++)
    // {
    //     c_current += 1;
    //     pthread_create(&c_tid[j], NULL, Consumer, (int)j);
    // }
    for (i = 0; i < p_count; i++)
    {
        pthread_join(p_tid[i], NULL);
    }
    for (i = 0; i < p_count * item_num; i++)
    {
        printf("%d\n", Buf.buf[i]);
    }
    // for (j = 0; j < c_count; j++)
    // {
    //     pthread_join(c_tid[j], NULL);
    // }
    clean_buf(&Buf);
    return 0;
}