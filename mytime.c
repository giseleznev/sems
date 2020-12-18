#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
 
int main(int argc, char *argv[])
{
	struct timespec tm1, tm2;
 
	clock_gettime (CLOCK_REALTIME, &tm1);
	pid_t pid = fork();

	if (pid == 0)
	{
		execvp(argv[1], argv + 1);
		return 0;
	}
	else
	{
		int stat;
		while (wait(&stat) != -1)
		{
			usleep(1);
		}
		clock_gettime (CLOCK_REALTIME, &tm2);
		int dt = (tm2.tv_sec - tm1.tv_sec) * 1000 +
				 (tm2.tv_nsec - tm1.tv_nsec) / 1000000;
		int dts = dt / 1000, dtm = dt % 1000;
		printf("Затрачено время: %d с %d mс\n", dts, dtm);
	}
	return 0;
}
