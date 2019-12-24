#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>

#define MAX_BUF 1024

int main()
{
    int fd;
    char * myfifo = "/tmp/myfifo";
    char buf[MAX_BUF];


    fd = open(myfifo, O_RDONLY);
    while (strcmp(buf, "quit\n")) {
           if(read(fd, buf, sizeof(buf))>0){
               printf("Received: %s\n", buf);
           }
    }
    close(fd);

    return 0;
}