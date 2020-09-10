#include <stdio.h>
#include <string.h>

int main(int ac, char *av[])
{
	int NoLine = 0;

	if(ac == 1){
		printf("\n");
		return 0;
	}

	if (strcmp(av[1], "-n") == 0){
		NoLine = 1;
	}

	for (int i = 1 + NoLine; i < ac; i ++) {
		printf("%s", av[i]);
		if (i != ac - 1){
			printf(" ");
		}
	}

	if (!NoLine) {
		printf("\n");
	}
	return 0;
}


