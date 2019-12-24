
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define M 4 //num of WR
#define N 3 //num of RE
sem_t accessM,readresM,orderM; //sem.
unsigned int readers = 0;	// Number of readers accessing the resource

void *reader(void *prm)
{
    int num1=*(int*)prm;
    while(1){

        if (sem_wait(&orderM)==0) printf("Читатель %d в очереди\n",num1);	// Remember our order of arrival
        sem_wait(&readresM);				 // We will manipulate the readers counter
        if (readers == 0)				// If there are currently no readers (we came first)...
            sem_wait(&accessM);				// ...requests exclusive access to the resource for readers
        readers++;							 // Note that there is now one more reader
        sem_post(&orderM);					 // Release order of arrival semaphore (we have been served)
        sem_post(&readresM);				 // We are done accessing the number of readers for now

        printf("Работает читатель %d\n",num1);				// Here the reader can read the resource at will
        sem_wait(&readresM);				 // We will manipulate the readers counter
        readers--;							 // We are leaving, there is one less reader
        if (readers == 0)				// If there are no more readers currently reading...
            sem_post(&accessM);				// ...release exclusive access to the resource
        sem_post(&readresM);				 // We are done accessing the number of readers for now
    }
}

void *writer(void *prm)
{
    int num2=*(int*)prm;
    while(1){
        if(sem_wait(&orderM)==0) printf("Писатель %d в очереди\n",num2); // Remember our order of arrival
        sem_wait(&accessM);					// Request exclusive access to the resource
        sem_post(&orderM);					 // Release order of arrival semaphore (we have been served)

        printf("Работает писатель %d\n",num2);				 // Here the writer can modify the resource at will
        sem_post(&accessM);					// Release exclusive access to the resource
    }
}

int main()
{
    pthread_t threadRE[N];
    pthread_t threadWR[M];
    sem_init(&accessM,0,1);
    sem_init(&readresM,0,1);
    sem_init(&orderM,0,1);

    printf("                         ОЧЕРЕДЬ/ВЫПОЛНЕНИЕ\n");
    int i;
    for(i=0;i<M;i++)
    {
        pthread_create(&(threadWR[i]),NULL,writer,(void*)&i);
    }
    for(i=0;i<N;i++)
    {
        pthread_create(&(threadRE[i]),NULL,reader,(void*)&i);
    }


    for(i=0;i<N;i++)
    {
        pthread_join(threadRE[i],NULL);
    }
    for(i=0;i<M;i++)
    {
        pthread_join(threadWR[i],NULL);
    }

    sem_destroy(&accessM);
    sem_destroy(&readresM);
    sem_destroy(&orderM);
}