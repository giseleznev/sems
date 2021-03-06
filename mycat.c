#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void Cat(const int fd) {
    char str[4096];
    int n = read(fd, str, 4096), p = 0;
    if (n < 0) {
        perror("cat: Permission denied");
        exit(1);
    }
    while (n > 0) {
        p = write(1, str, n);
        if (p < 0) {
            perror("cat: Permission denied");
            exit(1);
        }
        n -= p;
    }
}

//нужно добавить еще флаги
//и аккуратный разбор флагов

int main(int argc, char const *argv[]) {

    if (argc == 1) {
        while (1)
            Cat(0);
    }

    for (int i = 1; i < argc; i++) {
        int fd;
        if ((fd= open(argv[i], O_RDWR | O_EXCL)) < 0) {
            perror("open failed");
            exit(1);
        }
        Cat(fd);
        close(fd);
    }

    return 0;
}