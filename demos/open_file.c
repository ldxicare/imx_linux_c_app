#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	int fd1, fd2, fd3;
	int ret;
	/* 第一次打开文件 */
	fd1 = open("./test_file", O_RDWR);
	if (-1 == fd1) {
		perror("open error");
		exit(-1);
	}
	/* 第二次打开文件 */
	fd2 = open("./test_file", O_RDWR);
	if (-1 == fd2) {
		perror("open error");
		ret = -1;
		goto err1;
	}
	/* 第三次打开文件 */
	fd3 = open("./test_file", O_RDWR);
	if (-1 == fd3) {
		perror("open error");
		ret = -1;
		goto err2;
	}
	/* 打印出 3 个文件描述符 */
	printf("%d %d %d\n", fd1, fd2, fd3);
	close(fd3);
	ret = 0;
err2:
	close(fd2);
err1:
	/* 关闭文件 */
	close(fd1);
	exit(ret);
}
