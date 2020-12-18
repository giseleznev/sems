#include <stdio.h>
#include <signal.h>
#include <sys/ioctl.h>

void border(int sig) {
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);
    //В качестве аргумента fd должен быть указан открытый файловый дескриптор,
    // TIOCGWINSZ - параметр действия, позволяет записать данные в ws
    //ws - void* место для записи данных
    printf("x: %d y: %d\n", ws.ws_col, ws.ws_row);
}

int main() {
    border(1);
    signal(SIGWINCH, border);
    //«SIGWINCH» — сигнал, посылаемый программе при изменении размеров терминала
    char q = 1;
    while (q != 'q')
        q = getchar();
    return 0;
}