#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void) 
{
	pid_t pid;
	pid = fork();

//	printf("pid = %d\n",pid);
	switch (pid) {
		case -1:
			perror("fork error");
			exit(-1);
		case 0:
			printf("等待子进程睡醒\n");
			sleep(3);
			printf("这是子进程打印信息<pid: %d, 父进程 pid: %d>\n",
					getpid(), getppid());
			_exit(0); //子进程使用_exit()退出
		default:
			printf("这是父进程打印信息<pid: %d, 子进程 pid: %d>\n",
					getpid(), pid);
			exit(0);
	}
}
