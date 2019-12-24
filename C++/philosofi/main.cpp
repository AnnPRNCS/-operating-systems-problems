#include <cstdio>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define LEFT (i+N-1) %N /* номер левого соседа для i-го философа */
#define RIGHT (i+1) %N /* номер правого соседа для i-го
философа */
#define N 6 /* количество философов */
#define THINKING 0 /* философ размышляет */
#define HUNGRY 1 /* философ пытается взять вилки */
#define EATING 2 /* философ ест спагетти */
int state[N]; /* массив для отслеживания состояния
каждого философа */
pthread_mutex_t mutex; /* Взаимное исключение входа в
критическую область */
sem_t s[N]; /* по одному семафору на каждого
философа */
struct id_philosopher{
    int id;
};
void test(int i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;
        sem_post(&s[i]);
    }
}
void think(int i){
    printf("Филосов №%d думает\n", i);
}
void eat(int i){
    printf("Филосов №%d ест\n", i);
}
void take_forks(int i) /* i – номер философа (от 0 до N−1) */
{
    pthread_mutex_lock(&mutex); /* вход в критическую область */
    state[i] = HUNGRY; /* запись факта стремления философа
поесть */
    test(i); /* попытка взять две вилки */
    pthread_mutex_unlock(&mutex); /* выход из критической области */
    sem_wait(&s[i]); /* блокирование, если вилки взять не
удалось */
}
void put_forks(int i) /* i – номер философа (от 0 до N−1) */
{
    pthread_mutex_lock(&mutex); /* вход в критическую область */
    state[i] = THINKING; /* философ наелся */
    test(LEFT); /* проверка готовности к еде соседа
слева */
    test(RIGHT); /* проверка готовности к еде соседа
*/
    pthread_mutex_unlock(&mutex); /* выход из критической области */
}

void* philosopher(void *argv)
{
    auto* information = (struct id_philosopher*)argv;
    int i = information->id;
    while (1) { /* бесконечный цикл */
        think(i); /* философ размышляет */
        take_forks(i); /* берет две вилки или блокируется */
        eat(i); /* ест спагетти */

        put_forks(i); /* кладет обе вилки на стол */
    }
}

int main(){
    pthread_t philosophers [N];
    pthread_mutex_init(&mutex, nullptr);
    id_philosopher information[N];
    for(int i=0;i<N;i++){
        information[i].id=i;
        pthread_create(&philosophers[i], NULL, &philosopher, &information[i]);
        sem_init(&s[i],0,0);
    }
    for(int i=0;i<N;i++){
        pthread_join(philosophers[i],NULL);
    }


}

