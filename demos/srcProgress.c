#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

extern char **environ;

static void bye(void) {
	puts("laidaixi-->Goodbye!");
}
int main(int argc, char *argv[])
{
	int i;
	
	for(i=0; NULL!=environ[i]; i++){
		puts(environ[i]);
	}
	pid_t pid = getpid(); //获取本进程 pid
	printf("本进程的 PID 为: %d\n", pid);
	pid = getppid(); //获取父进程 pid
	printf("父进程的 PID 为: %d\n", pid);

	if (atexit(bye)) {
		fprintf(stderr, "cannot set exit function\n");
		exit(-1);
	}
	exit(0);
}
