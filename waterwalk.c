#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int n; //людей
int k; //на корабле
int m;

int id;
 
void Human(int i){
		//ждёт посадки, уходит с берега, занимает место на трапе, занимает место на корабле, освобождает место на трапе
		struct sembuf Array1[6] = {{1, 0, 0}, {0, -1, 0}, {2, -1 ,0}, {3, -1 ,0}, {6, 1, 0}, {2, 1 ,0}};
	
		printf("human1%i: %d\n", i, semop(id, Array1, 6));
	//	semop(id, Array1, 6);
		printf("%d, is on board\n", i);
		
		//определить, что плывет
		struct sembuf Array3[1] = {{5, 0, 0}};
		
		//semop(id, Array3, 1);
				
		printf("human2%i: %d\n", i, semop(id, Array3, 1));
		
		printf("%d is enjoying waterwalk!\n", i);

		//ждёт высадки, занимает место на трапе, освобождает место на корабле, приходит на берег
		struct sembuf Array2[6] = {{4, 0, 0}, {2, -1 ,0}, {3, 1 ,0}, {6, -1, 0}, {2, 1 ,0}, {0, 1, 0}};
		printf("human3%i: %d\n", i, semop(id, Array2, 6));

		printf("%d, hop off\n", i);
}

void Cap(){
	
	printf("Boarding!\n");
	
	struct sembuf ArrayP[1] = {{1, -1, 0}}; //открыть посадку
	semop(id, ArrayP, 1);
	
	struct sembuf Array1[1] = {{0, 0, 0}}; //можно отправляться т.к. закончились люди
	struct sembuf Array2[1] = {{3, 0, 0}}; //можно отправляться т.к. закончилось место
	struct sembuf Array3[2] = {{1, 1, 0}, {5, -1, 0}}; //зыкрыть посадку, отплытие
	struct sembuf ArrayV[2] = {{5, 1, 0}, {4, -1, 0}}; //закончить плаванье, открыть высадку
	struct sembuf ArrayV1[1] = {{6, 0, 0}}; //если все вышли 
	struct sembuf ArrayV2[1] = {{4, 1, 0}};//закрыть высадку
	
	
	//semop(id, Array1, 1);
	
	if (k > n){
		printf("Cap: %d\n", semop(id, Array1, 1));
	} else {
		printf("Cap: %d\n", semop(id, Array2, 1));
	};

	printf("Boarding ends. Let's go! \n");
	
	printf("Cap2:%d\n", semop(id, Array3, 2)); 
	
	printf("arrival, you can hop off \n");

	printf("Cap3:%d\n", semop(id, ArrayV, 2));
	
	printf("Cap4:%d\n", semop(id, ArrayV1, 1));
	printf("Cap5:%d\n", semop(id, ArrayV2, 1));
	
	printf("end\n");

}

union semun {
      int val;                  /* value for SETVAL */
      struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
      unsigned short *array;    /* array for GETALL, SETALL */
                                /* Linux specific part: */
      struct seminfo *__buf;    /* buffer for IPC_INFO */
};

int main(int argc, char *argv[])
{ 
	setbuf(stdout, NULL);
	
	id = semget(IPC_PRIVATE, 7, (IPC_CREAT|0700)); 
	printf("%d\n", id);
	
	int n = atoi(argv[1]); //людей
	int k = atoi(argv[2]); //на корабле
	int m = atoi(argv[3]); //на трапе
	
	semctl(id, 0, SETVAL, n); // люди
	semctl(id, 1, SETVAL, 1); // посадка
	semctl(id, 2, SETVAL, m); // трап
	semctl(id, 3, SETVAL, k); // свободных мест на корабле
	semctl(id, 4, SETVAL, 1); // высадка
	semctl(id, 5, SETVAL, 1); // плывем(0 - да, 1 - нет)
	semctl(id, 6, SETVAL, 0); // занятых мест на корабле
	
	for(int i = 0; i < n + 1; i++){
		if (fork() == 0){
			if( i == 0 ){
				while(1){
				Cap();	
				}
				exit(1);
			}
			else {
				while(1){
				Human(i);
				}
				exit(1);
			}
		}    
	}
	wait(0);
	semctl(id, IPC_RMID, 0);
	return 0;
}
