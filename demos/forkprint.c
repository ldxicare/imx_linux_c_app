#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int g_counter;
int main(void) 
{
	g_counter++;
	printf("Hello World!-->%d",g_counter);
	sleep(3);
	switch (fork()) {
		case -1:
			perror("fork error");
			exit(-1);
		case 0:
			/* 子进程 */
			printf("子进程exit\n");
			_exit(0);
		default:
			/* 父进程 */
			printf("父进程exit\n");
			exit(0);
	} 
}
