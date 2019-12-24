//
// Created by hal4dan on 28.09.2019.
//
#include <stdio.h>
#include <fcntl.h>

#include <sys/types.h>

#include <sys/stat.h>
#include <zconf.h>
#include <cstdlib>


int main() {

    int fd;

    int x, y;

    pid_t pid;

    unlink("file"); /*Удаляем предыдущий файл*/

    fd = open("file", O_WRONLY | O_CREAT, 0777);

    if (fd == -1) {

        perror("open : ");

        exit(0);

    }

    if ((pid = fork()) == -1) {

        perror("fork :");

        exit(0);

    } else if (pid) { /*Родительский процесс*/

        while (1) {

            for (x = 0; x < 10; x++) {

                sleep(1);

                write(fd, (char *) "x", 1);

            }

            break;

        }
    } else { /*Процесс-потомок*/

        while (1) {

            for (y = 0; y < 10; y++) {

                sleep(1);

                write(fd, (char *) "X", 1);

            }

            break;

        }

        return 0;

    }
}