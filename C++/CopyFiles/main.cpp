#include <iostream>
#include <cstdlib>     /* atoi */
#include <zconf.h>

using namespace std;

int main(int argc, char **argv) {
    size_t sz = atoi(argv[1]);
    char *buffer = new char[sz];
    ssize_t fl = 1;
    while (fl != 0) {
        fl = read(STDIN_FILENO, buffer, sz);
        if (fl == -1) {
            cout << "ошибка чтения";
        }
        if (fl != 0) {
            if (write(STDOUT_FILENO, buffer, fl) == -1) {
                cout << "ошибка записи";
            }
        }
    }
    delete[] buffer;
    return 0;
}