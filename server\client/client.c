#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <fcntl.h>

int Enter(int id, int semnum) {
    struct sembuf sops = {semnum, 1, 0};
    return semop(id, &sops, 1);
}

int Exit(int id, int semnum) {
    struct sembuf sops = {semnum, -1, 0};
    return semop(id, &sops, 1);
}

int Wait(int id, int semnum) {
    struct sembuf sops = {semnum, 0, 0};
    return semop(id, &sops, 1);
}

void client_c(const int fd, int shmid, int semid)
{
    FILE *stream = fdopen(fd, "r"); // открываем дискриптор как поток для чтения по строкам

    char *addr;

    addr = (char *)shmat(shmid, NULL, 0); // указатель на общую память

    while(!feof(stream)) {
        //printf("wait\n");

        if (Wait(semid, 0)!= 0){
            perror("wait!");
        }; //ждем пока печатают

        if (Exit(semid, 1)!= 0){
            perror("Exit!");
        };
         // идёт запись

        printf("%s", fgets(addr, 60, stream)); //записываем строку в общую память
        printf("was written: %c\n", *addr);

        Enter(semid, 1); // идёт запись
    }

    fclose(stream);
}

int main(int argc, char *argv[]) {
    char *path = "client.c";

    key_t key = ftok(path, 1);

    int id_m = shmget(key, 256, 0666|IPC_CREAT);

    int id_s = semget(key, 2, 0666|IPC_CREAT);

    semctl(id_s, 0, SETVAL, 0); // люди
    semctl(id_s, 1, SETVAL, 1); // посадка

    char *p = shmat(id_m, NULL, 0);

    while (1) {
        if (argc == 1) {
            client_c(0, id_m, id_s);

        } else {
            for (int i = 1; i < argc; i++) {
                int fd = open(argv[i], O_RDWR | O_EXCL); //связываем дискриптор с файлом

                client_c(fd, id_m, id_s);

                close(fd);
            }
        }
    }
}