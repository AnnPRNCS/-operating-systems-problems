#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define COUNT_CLIENTS 6
#define CHAIRS 5
sem_t customers;
sem_t barbers;
pthread_mutex_t mutex;

int waiting = 0;
void cut_hair(){
printf("Барбер стрижет\n");
};
void get_haircut(){
    printf("Клиент стрижется\n");
};
void* Barber(void* argv)
{
    while(1)
    {
        sem_wait(&customers);
        pthread_mutex_lock(&mutex);
        waiting=waiting-1;
        sem_post(&barbers);
        pthread_mutex_unlock(&mutex);
        cut_hair();
    }
}
void* customer(void* argv)
{
    while(1) {
        pthread_mutex_lock(&mutex);
        if (waiting < CHAIRS) {
            waiting = waiting + 1;
            sem_post(&customers);
            pthread_mutex_unlock(&mutex);
            sem_wait(&barbers);
            get_haircut();
        } else {
            pthread_mutex_unlock(&mutex);
        }
    }
}
int main(){
    pthread_t barber;
    pthread_t clients [COUNT_CLIENTS];

    pthread_mutex_init(&mutex,NULL);

    sem_init(&customers,0,0);
    sem_init(&barbers,0,0);

    pthread_create(&barber,NULL,&Barber, NULL);
    for(int i=0;i<COUNT_CLIENTS;i++){
        pthread_t id;
        pthread_create(&id,NULL, &customer,(void *)i);
        clients[i]=id;
    }

    pthread_join(barber, NULL);
    for(int i=0;i<COUNT_CLIENTS;i++){
        pthread_join(clients[i],NULL);
    }

}