#include <fcntl.h>
#include <zconf.h>
#include <cstdlib>
#include <iostream>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main() {
    int fd;
    if ((fd = creat("file.hole", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
        std::cerr << "ошибка вызова creat";
    if (write(fd, buf1, 10) != 10)
        std::cerr << "ошибка записи buf1";
/* теперь текущая позиция = 10 */
    if (lseek(fd, 16384, SEEK_SET) == -1)
        std::cerr << "ошибка вызова lseek";
/* теперь текущая позиция = 16384 */
    if (write(fd, buf2, 10) != 10)
        std::cerr << "ошибка записи buf2";
/* теперь текущая позиция = 16394 */
    exit(0);
}