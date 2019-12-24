#include <iostream>
#include <sys/stat.h>
#include <cstring>
#include <dirent.h>
#include <zconf.h>

using namespace std;

/**
 * Метод рекурсивно сканирует дерево каталогов вверх до корня, подсчитывая количество символических ссылок на указанную
 * папку, лежащих по пути до этой папки.
 * @example если указанная папка /home/username/symlinksWorkingDir, то метод будет сканировать каждый уровень
 * (/, home, username, symlinksWorkingDir), проверяя каждый файл на уровне. Если, например, файл /symlink - ссылка на
 * директорию /home/username/symlinksWorkingDir, то он будет посчитан. Если файл /bin/symlink - символическая ссылка
 * на /home/username/symlinksWorkingDir, то он не будет посчитан, т.к. он не лежит на пути /home/username/symlinksWorkingDir
 *
 * @param path абсолютный путь до папки.
 * @param rootStat данные о корневом каталоге. Нужны для понимая того, что рекурсивный метод дошел до корня.
 * @param iNode идентификатор папки.
 * @param isRootScanned @code {true}, если корень уже просканирован.
 * @return количество символических ссылок на папку, лежащих на пути от корня до папки.
 */
int getNumSymbolic(char *path, struct stat rootStat, int iNode, bool isRootScanned);

int main() {
    char rootPath[100] = "/";

    // получаем текущую рабочую директорию.
    char path[256] = {0};
    getcwd(path, 256);

    // запоминаем данные о корне.
    struct stat rootStat;
    lstat(rootPath, &rootStat);
    cout << "root=/, rootINode=" << rootStat.st_ino << ", rootDev=" << rootStat.st_dev << endl;

    // запоминаем данные о рабочей директории.
    struct stat workingDirStat;
    lstat(path, &workingDirStat);
    cout << "working dir=" << path << ", wDirINode=" << workingDirStat.st_ino << ", wDirDev=" << workingDirStat.st_dev
         << endl;

    auto numSymlinks = getNumSymbolic(path, rootStat, workingDirStat.st_ino, false);
    cout << "Find " << numSymlinks << " symbolic links" << endl;


    return 0;
}

int getNumSymbolic(char *path, const struct stat rootStat, const int iNode, const bool isRootScanned) {

    int numSymbolicLinks = 0;
    bool isRoot = false;

    const char *dot = "/..";
    const char *slash = "/";

    // открываем папку.
    DIR *dir = opendir(path);
    // указатель на элемент, лежащий в папке.
    struct dirent *dp;
    // читаем все файлы папки. Признак того, что файлы закончились - nullptr.
    while ((dp = readdir(dir)) != nullptr) {
        // в следующих 4 строках создается путь до очередного файла. Например, если просматривается файл symlink в папке /home,
        // то надо к /home добавить / и имя файла => /home/symlink - путь до этого файла.
        char *fullPath = new char[255];
        fullPath[0] = 0;
        strcpy(fullPath, path);
        strcat(fullPath, slash);
        strcat(fullPath, dp->d_name);

        // получаем данные об очередном файле.
        struct stat curFileStat;
        lstat(fullPath, &curFileStat);

        // здесь мы проверям, что очередной файл корень. Признак того, что файл корень - совпадение ino & dev.
        if (curFileStat.st_ino == rootStat.st_ino && curFileStat.st_dev == rootStat.st_dev) {
            isRoot = true;
        }

        // выводим на консоль имя очередного файла и его ino. Можно и не выводить.
        cout << fullPath << " " << curFileStat.st_ino << endl;

        // S_LINK возвращает ненулевое значение, если файл - символическая ссылка.
        if (S_ISLNK(curFileStat.st_mode)) {
            char buf[255];
            // читаем ссылку, т.е. тот файл, на который она указывает.
            int len = readlink(fullPath, buf, 255);
            buf[len] = 0;
            // читаем инфу о файле, на который указывает ссылка.
            lstat(buf, &curFileStat);
            if (len == -1) {
                // возникла ошибка при чтении ссылки - пишем это в лог.
                cerr << "Error with Symbolic  link" << endl;
            } else if (curFileStat.st_ino == iNode) {
                // ура - нашли нужную нам ссылку.
                numSymbolicLinks++;
                cout << "find right symlink: " << fullPath << endl;
            }
        }
    }
    cout << endl << endl << endl;

    if (!isRoot) {
        // если корень не достигнут - поднимается на уровень вверх и сканируем следующий уровень.
        strcat(path, dot);
        numSymbolicLinks += getNumSymbolic(path, rootStat, iNode, false);
    } else if (!isRootScanned) {
        // вот тут интересно - корень уже найден, но еще не просканирован. Для того, чтобы не упасть в бесконечный цикл,
        // проверящий корень, и введен флажок isRootScanned. Если он выставлен в true, то это значит, что корень уже
        // просмотрен и 2 раз его смотреть не надо.
        strcat(path, dot);
        numSymbolicLinks += getNumSymbolic(path, rootStat, iNode, true);
    }
    return numSymbolicLinks;
}


