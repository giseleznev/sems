#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
	long mtype;   /* тип сообщения, должен быть > 0 */
	char mtext[1];        /* содержание сообщения */
};

struct Message_t
{
  long value;
};

 
int main(int argc, char *argv[])
{
	int N = atoi(argv[1]);
		
	struct timespec start_clock, final_clock;
	//clock_gettime (CLOCK_REALTIME, &tm1);
	
	int id = msgget(IPC_PRIVATE, 0777);
	
	printf("msgquew: %d \n", id);
	
	struct Message_t Message;
	
	int msg_size = sizeof(Message);
	
	
			pid_t pid = fork();
	
	if (pid == 0){
		
		Message.value = 1;
		
		if(msgsnd(id, &Message, msg_size, 0)<0){
			printf("Fuck up. %s \n", strerror(errno));
			};
		
		if(msgrcv(id, & Message, msg_size, N + 1, 0) < 0) {
			perror("ERROR");
			};
		
			printf("everybody is here \n");
			printf("Run! \n");
		
		Message.value = N + 2;
			msgsnd(id, &Message, msg_size, 0);
		
		
		clock_gettime (CLOCK_REALTIME, &start_clock);
						
		if(msgrcv(id, &Message, msg_size, N + N + 2, 0)<0) {
			perror("ERROR");
			};
			
		printf("Finish! \n");
		
		clock_gettime (CLOCK_REALTIME, &final_clock);
		
		double secs = final_clock.tv_sec - start_clock.tv_sec;
  double nsecs = final_clock.tv_nsec - start_clock.tv_nsec;

  double time = secs*1000 + nsecs/1000000;

  printf("Process time in msecs : %lf\n", time);
		
		Message.value = 3*N;
		
	for (int i = 1; i < N + 1; i++){	
		msgsnd(id, &Message, msg_size, 0);
	}
		exit(0);
	}
	
	
	for (int i = 1; i < N + 1; i++){
		if (fork() == 0){
			if(msgrcv(id, &Message, msg_size, i, 0)<0) {
			perror("ERROR");
			};
			
			printf("%d is here \n", i);
			
			Message.value = i+1;
			
			msgsnd(id, &Message, msg_size, 0); //4 если бегунов < 10^4 , flag = 0	
					
			msgrcv(id, &Message, msg_size, N + i + 1, 0);
			
			printf("%d run \n", i);
					
			Message.value = N + i + 2;
			
			msgsnd(id, &Message, msg_size, 0);
			
			msgrcv(id, &Message, msg_size, 3*N, 0);
			
			printf("%d goes home \n", i);
			
			return 0;
		}
	}	
	wait(0);
	msgctl(id, IPC_RMID, 0);
	return 0;
}
