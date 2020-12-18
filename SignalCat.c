#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int out_char = 0, counter = 128;
pid_t pid;

// Объявляем функции выполняемые по сигналам

// SIGCHLD
void childexit(int signo) {
    exit(EXIT_SUCCESS);
}

// SIGALRM
void parentexit(int signo) {
    exit(EXIT_SUCCESS);
}

// Nothing
void empty(int signo) {
}

// SIGUSR1
void one(int signo) {
    out_char += counter;
    counter /= 2;
    kill(pid, SIGUSR1);
}

// SIGUSR2
void zero(int signo) {
    counter/=2;
    kill(pid, SIGUSR1);
}

void SignalCat(int fd){
    pid_t ppid = getpid(); //Запонимаем пид родителя, то есть приёмника

    sigset_t set;// Изменяем набор блокированых сигналов

    // при SIGCHLD - выходим
    struct sigaction act_exit;
    memset(&act_exit, 0, sizeof(act_exit));
    act_exit.sa_handler = childexit;
    sigfillset(&act_exit.sa_mask);
    sigaction(SIGCHLD, &act_exit, NULL);

    // SIGUSR1 - one()
    struct sigaction act_one;
    memset(&act_one, 0, sizeof(act_one));
    act_one.sa_handler = one;
    sigfillset(&act_one.sa_mask);
    sigaction(SIGUSR1, &act_one, NULL);

    // SIGUSR2 - zero()
    struct sigaction act_zero;
    memset(&act_zero, 0, sizeof(act_zero));
    act_zero.sa_handler = zero;
    sigfillset(&act_zero.sa_mask);
    sigaction(SIGUSR2, &act_zero, NULL);

    // Добавляем блокировки
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGCHLD);

    sigprocmask(SIG_BLOCK, &set, NULL );
    sigemptyset(&set);

    if ( fork() == 0) {

        char c = 0;
        sigemptyset(&set); // очищает набор сигналов

        // SIGUSR1 - empty()
        struct sigaction act_empty;
        memset(&act_empty, 0, sizeof(act_empty));
        act_empty.sa_handler = empty;
        sigfillset(&act_empty.sa_mask);
        sigaction(SIGUSR1, &act_empty, NULL);

        // SIGALRM - parentexit()
        struct sigaction act_alarm;
        memset(&act_alarm, 0, sizeof(act_alarm));
        act_alarm.sa_handler = parentexit;
        sigfillset(&act_alarm.sa_mask);
        sigaction(SIGALRM, &act_alarm, NULL);

        while (read(fd, &c, 1) > 0){
            // SIGALRM Будет получен если родитель не успеет ответить за секунду
         //   alarm(1);
            // Побитовые операции
            for (int i = 128; i >= 1; i /= 2){
                if ( i && c ) {            // 1
                    kill(ppid, SIGUSR1);
                    printf("give 1");
                }
                else{                      // 0

                    kill(ppid, SIGUSR2);// Ждём подтверждения от родителя // приостанавливает процесс до получения сигнала
                    printf("give 1");
                }
                sigsuspend(&set);
            }
        }// Файл кончился
        exit(EXIT_SUCCESS);
    }
    else { // Получаем пока ребёнок не умрёт



        while (1) {
 //           sigprocmask(SIG_UNBLOCK, &set, NULL );
            if (counter == 0) {       // Whole byte
                write(1, &out_char, 1);  //
                printf("%d\n", out_char);
                fflush(stdout);
                counter = 128;
                out_char = 0;
            }
            sigsuspend(&set); // Ждём сигнал от ребёнка
        }
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char * argv[]){
    if (argc == 1) {
        while (1)
            SignalCat(0);
    }

    for (int i = 1; i < argc; i++) {
        int fd;
        if ((fd = open(argv[i], O_RDWR | O_EXCL)) < 0) {
            perror("open failed");
            exit(1);
        }
        SignalCat(fd);
        close(fd);
    }
}