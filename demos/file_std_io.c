#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	printf("print:");
	write(STDOUT_FILENO,"write\n",6);
	exit(0);
}
