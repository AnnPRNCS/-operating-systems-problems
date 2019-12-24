#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

#define BUF_LEN 50
int main()
{   int msg_len;
    int fd;
    char * myfifo = "/tmp/myfifo";
    char BUFFER[BUF_LEN];
    mkfifo(myfifo, 0666);


    fd = open(myfifo, O_WRONLY);
    while(strcmp(BUFFER, "quit\n")) {
        fgets(BUFFER, BUF_LEN, stdin);
        msg_len = strlen(BUFFER) + 1;
        write(fd, BUFFER , msg_len);
    }
    close(fd);

    unlink(myfifo);

    return 0;
}