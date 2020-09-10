#include"string.h"
#include"unistd.h"

int main(){
	char* out = "Hello World\n";	
	write(1, out, strlen(out));
	return 0;
}
