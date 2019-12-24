#include "cstdio"
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

const int M = 5;//размер буфера
const int N = 2;//число потребителей

int arr[M];
int pos = 0;//позиция в буфере откуда читаем и пишем
int valConsume = 0;//позиция в буфере откуда читаем и пишем
int valProduce = M;//позиция в буфере откуда читаем и пишем

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//чтобы организовать взаимоисключение для буфера
pthread_mutex_t mutexCons = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexProd = PTHREAD_MUTEX_INITIALIZER;
void * consume(void *arg) {;
    while (1) {
        int localItem;//куда читаем элемент из буфера

        while (valConsume == 0){
        }

        if (pos>0) {
            pthread_mutex_lock(&mutex);
            pos--;
            localItem = arr[pos];
            pthread_mutex_unlock(&mutex);
            pthread_mutex_lock(&mutexCons);
            valConsume--;
            pthread_mutex_unlock(&mutexCons);
            pthread_mutex_lock(&mutexProd);
            valProduce++;
            pthread_mutex_unlock(&mutexProd);
            printf("Consumed\n");
        }
    }
}

void * produce(void * arg){
    while(1) {
        while(valProduce == 0) {}
        int item = rand() % 100;

        pthread_mutex_lock(&mutex);//опускаем мьютекс для организаци взаимоисключения
        arr[pos] = item;
        pos++;
        pthread_mutex_unlock(&mutex);//разрешили заходить в критическую секцию

        pthread_mutex_lock(&mutexCons);//разрешили заходить в критическую секцию
        valConsume++;
        pthread_mutex_unlock(&mutexCons);//разрешили заходить в критическую секцию

        pthread_mutex_lock(&mutexProd);//разрешили заходить в критическую секцию
        valProduce--;
        pthread_mutex_unlock(&mutexProd);//разрешили заходить в критическую секцию

        printf("Produced\n");
    }
}

int main(int argumentsCount, char** args) {
    srand(time(0));
    pthread_t producer;
    pthread_t consumer[N];// объявляем переменные потока
    void * exitCode;// переменная ошибки потока

    pthread_create(&producer, NULL, produce, NULL);

    for(int i = 0; i<N; i++){
        pthread_create(&consumer[i], NULL, consume, NULL);
    }

    for(int i = 0; i < N; i++) {
        pthread_join(consumer[i], &exitCode);// запускаем поток. join = нельзя завершать основной поток, пока не завершен наш
    }
    pthread_join(producer, &exitCode);

    return 0;
}