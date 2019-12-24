#include <cstdio>
#include <cstring>
#include <pthread.h>
#include <sched.h>
#include <curses.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <zconf.h>
#include <cstdlib>

// длина буфера
const int BUF_LEN = 5;

// Объект синхронизации типа pthread_mutex_t
pthread_mutex_t mutex;
// Признак окончания работы по Ctrl-D
int done;
// Длина сообщения
unsigned int msg_len;
// Буфер для сообщения
char BUFFER[BUF_LEN];
// файловый дискриптор
int fd;

/* Считать сообщение и поместить его в буфер.
Функция работает как независимая задача, вызываемая операционной системой,
поэтому прототип фиксирован. Аргумент argp не используется. */
void *reader(void *argp) {
    printf("Reader thread %x start working\n", (int) pthread_self());
    while (TRUE) {
        // "захватить" mutex
        pthread_mutex_lock(&mutex);
        //если длина равна нулю
        if (!msg_len) {
            // Считать сообщение. Выход no Ctrl+D
            if (!fgets(BUFFER, BUF_LEN, stdin)) {
                done = 1;
                break;
            }
            msg_len = strlen(BUFFER) + 1;

            // выход по quit
            if (strcmp(BUFFER, "quit") == 0) {
                printf("Reader thread %x stopped by quit message\n", (int) pthread_self());
                break;
            }

        }
        // "освободить" mutex
        pthread_mutex_unlock(&mutex);
        // Поместить задачу (поток) в конец очереди готовых задач с тем же приоритетом
        sched_yield();
    }
    // "освободить" mutex
    pthread_mutex_unlock(&mutex);
    // завершить задачу
    return nullptr;
}

/* Ждать сообщения, по его поступлении вывести его на экран. Функция работает
как независимая задача, вызываемая операционной системой, поэтому прототип
фиксирован. Аргумент argp не используется. */
void *writer(void *argp) {

    printf("Writer thread %x start working\n", (int) pthread_self());

    while (TRUE) {
        // "захватить" mutex
        pthread_mutex_lock(&mutex);
        if (done) {
            // Выход по Ctrl-D
            pthread_mutex_unlock(&mutex);
            // завершить задачу
            return nullptr;
        }
        if (msg_len != 0) {
            // выход по сообщению quit
            if (strcmp(BUFFER, "quit") == 0) {
                printf("Writer thread %x stopped by quit message\n", (int) pthread_self());
                pthread_mutex_unlock(&mutex);
                return nullptr;
            }

            // вывести на экран
            printf("Writer thread %x write -> %s\n", (int) pthread_self(), BUFFER);
            // пишем в файл
            if (write(fd, BUFFER, strlen(BUFFER)) != strlen(BUFFER)) {
                fprintf(stderr, "Error with writing");
                exit(1);
            }
            msg_len = 0;
        }
        // "освободить" mutex
        pthread_mutex_unlock(&mutex);
        // Поместить задачу (поток) в конец очереди готовых задач
        // с тем же приоритетом
        sched_yield();
    }
}

int main() {

    if ((fd = creat("out.txt", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | O_TRUNC)) < 0) {
        fprintf(stderr, "Error creat file");
        return 5;
    }

    // дескрипторы задач (потоков)
    pthread_t reader_thread;
    // создать mutex
    if (pthread_mutex_init(&mutex, nullptr)) {
        fprintf(stderr, "Cannot init mutex\n");
        return 1;
    }

    // вектор писателей
    std::vector<pthread_t> writer_threads(5);



    // создать задачу reader
    if (pthread_create(&reader_thread, nullptr, reader, nullptr)) {
        fprintf(stderr, "Cannot create reader thread\n");
        return 2;
    }

    // создаем задачи writers
    for (auto &it: writer_threads) {
        if (pthread_create(&it, nullptr, writer, nullptr)) {
            fprintf(stderr, "Cannot create writer thread\n");
            return 3;
        }
    }


    // ждать окончания задачи reader_thread
    pthread_join(reader_thread, nullptr);

    // ждем окончания задач writer_threads
    for (auto &it: writer_threads) {
        if (pthread_join(it, nullptr)) {
            fprintf(stderr, "Cannot destroy mutex\n");
            return 4;
        }
    }

    // удалить mutex
    if (pthread_mutex_destroy(&mutex)) {
        fprintf(stderr, "Cannot destroy mutex\n");
        return 4;
    }
    return 0;
}