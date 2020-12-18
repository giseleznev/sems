#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
 
// Позиция ключа может быть только после myexecwc
int writer(int from, int to, int info);
 
int main(int argc, char const *argv[])
{ // https://habr.com/ru/post/55665/ - подробнее про разбор аргументов
	int wc = 0;
	const char* short_options = "w";  //набор возможных опций(если "w:", то с аргументом)
 
	const struct option long_options[] = { // набор возможных длинных опций
		{"wc", no_argument ,NULL,'w'},  // const char *name; int has_arg; int *flag(возвр val, когда NULL); 
		//int val(обычно содержит некоторую символьную константу, если длинная опция соответствует короткой, 
		// то эта константа должна быть такой же как и та что появляется в аргументе optstring);
		
		{NULL,0,NULL,0} //посл эл обязательно нулями
	};
 
	int rez;  //возвр -1. когда разбор закончен, и val когда натыкается на арг
	int option_index; // указывает на переменную, в которую помещается индекс обнаруженной длинной опции в longopts, 
	// если в это нет необходимости может быть NULL
 
 	while ((rez = getopt_long(argc,argv, short_options,
		long_options, &option_index))!=-1)
	{

		switch(rez)
		{
			case 'w': {
				wc = 1;
				break;
			};
			case '?': default: {
				printf("found unknown option\n");
				break;
			};
		};
	};

	if (wc)
	{
		int fds[2]; // канал - pipe связь файловых дискрипторов
		pipe(fds); //создаёт однон аправленный канал данных, который можно использовать для взаимодействия между процессами.
		//Массив fds используется для возврата двух файловых описателей, указывающих на концы канала. 
		//fds[0] указывает на конец канала для чтения. fds[1] указывает на конец канала для записи. 
	
		if(fork() == 0) //для дочернего процесса
			{
				dup2(fds[1], 1); // печать не в stdout, а в fds[1]!
				execvp(argv[2], argv + 2); // !если ключ есть 
				
				("argv[1] ");
			}
		else //для родительского процесса
		{
			int stat;
			while (wait(&stat) != -1)
			{
				usleep(1);
			}
			close(fds[1]); //Close writer
			writer(fds[0], 1, 1); // считать в буффер и посчитать строки, слова, байты
		}
	}
	return 0;
}
 
int writer(int from, int to, int info)
{
	if (from < 0)
	{
		perror("mycat");
		return 0;
 
	}
	else
	{
		size_t size = 4096;
		char buff[4096];
		ssize_t real_size = read(from, buff, size);
		ssize_t already_written = 0;
		printf(buff);
		if (info)
		{
			int i = 0;
			printf("%ld bytes ", real_size);
			int string_counter = 1;
			char character;
			for (i; i < real_size; i++)
			{				
				character = buff[i]
				if (isspace(character))
				{
					if (character == '\n')
					{
						string_counter += 1;
					}
				}
			}
			printf("strings %d ", string_counter);
		}
		else
		{
			while (already_written != real_size)
				{
					already_written += write(to, buff + already_written,
										  real_size - already_written);
				}
			return real_size;
		}
 
	}
 
}
