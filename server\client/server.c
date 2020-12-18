#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

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

int main(){
    //0 - печать    0 - печать закончена, 1  - идёт
    //1 - запись    0 - запись закончена, 1 - запись идёт
    char* path = "client.c";

    key_t key = ftok(path, 1);

	printf("key: %d", key);
	
	int id_m = shmget(key, 256, 0666|IPC_CREAT);
	int id_s = semget(key, 2, 0666|IPC_CREAT);
	
	semctl(id_s, 0, SETVAL, 0);
    semctl(id_s, 1, SETVAL, 0); // нач. знач.

    char* p = shmat(id_m, NULL, 0);

    while(1){
		Wait(id_s, 1); // ждём покуда не пройдет запись

        Enter(id_s, 0); //занимаем очередь печатью

        //printf("%s", p);

        if (*p != 0){
		    printf("%s", p); //напечатали то что в общей памяти
            *p = 0;
        }

		Exit(id_s, 0);	//освобождаем очередь от печати
    }
    //семефоры и память удаляются сами при экстренном закрытии
}
