#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(void) {
	int fd;
	int ret;
	char buffer[1024];
	int i,j;
	/* 打开文件 */
	fd = open("./test_file", O_WRONLY | O_CREAT | O_EXCL,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (-1 == fd) {
		perror("open error");
		exit(-1);
	}
	/* 循环写入 4 次，每次写入 1K */
	for (i = 0; i < 4; i++) {
		memset(buffer,'1'+i,sizeof(buffer));
		ret = write(fd, buffer, sizeof(buffer));
		if (-1 == ret) {
			perror("write error");
			goto err;
		}
	}
	ret = 0;
err:
	/* 关闭文件 */
	close(fd);
	exit(ret);
}
