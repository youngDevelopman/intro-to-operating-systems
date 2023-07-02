#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_READERS 4
#define NUM_WRITERS 2

pthread_mutex_t mutex;
pthread_cond_t turn;

int writing;
int reading;

int sharedData;

void *reader(void *arg) {
    int threadId = *(int *)arg;
    printf("Reader %d: Enterinjg the reading function\n", threadId);
    pthread_mutex_lock(&mutex);
    printf("Reader %d: Entered the critical section\n", threadId);
    while(writing){
        printf("Reader %d: Waiting for the writers to complete\n", threadId);
        pthread_cond_wait(&turn, &mutex);
    }
    reading++;
    pthread_mutex_unlock(&mutex);

    
    printf("Reader %d: Start reading...\n", threadId);
    int randomNum = (rand() % 16) + 5;
    sleep(randomNum);
    printf("Reader %d: Stop reading. Shared data = %d\n", threadId, sharedData);

    pthread_mutex_lock(&mutex);
    reading--;
    pthread_cond_signal(&turn);
    pthread_mutex_unlock(&mutex);
    printf("Reader %d: Exiting the reading\n", threadId);
    pthread_exit(NULL);
}

void *writer(void *arg) {
    int threadId = *(int *)arg;
    printf("Writer %d: Entering the writing function\n", threadId);
    pthread_mutex_lock(&mutex);
    printf("Writer %d: Entered the critical section\n", threadId);
    while(writing || reading){
        printf("Writer %d: Waiting for the readers and writers to complete\n", threadId);
        pthread_cond_wait(&turn, &mutex);
    }
    writing++;
    pthread_mutex_unlock(&mutex);

    printf("Writer %d: Start writing...\n", threadId);
    int randomNum = (rand() % 16) + 5;
    sleep(randomNum);
    sharedData++;
    printf("Writer %d: Stop writing. Shared data = %d\n", threadId, sharedData);

    pthread_mutex_lock(&mutex);
    writing--;
    pthread_cond_signal(&turn);
    pthread_mutex_unlock(&mutex);

    printf("Writer %d: Exiting the writing\n", threadId);
    pthread_exit(NULL);
}

int main(){
    srand(time(NULL));
    pthread_t readerThreads[NUM_READERS];
    pthread_t writerThreads[NUM_WRITERS];
    int readerIds[NUM_READERS];
    int writerIds[NUM_WRITERS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&turn, NULL);

    for (int i = 0; i < NUM_READERS; i++)
    {
        readerIds[i] = i + 1;
        pthread_create(&readerThreads[i], NULL, reader, &readerIds[i]);
    }
    for (int i = 0; i < NUM_WRITERS; i++)
    {
        writerIds[i] = i + 1;
        pthread_create(&writerThreads[i], NULL, writer, &writerIds[i]);
    }

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readerThreads[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writerThreads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&turn);

    return 0;
}