#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	struct stat file_stat;
	int ret;
	/* 获取文件属性 */
	ret = stat("./test_file", &file_stat);
	if (-1 == ret) {
		perror("stat error");
		exit(-1);
	}
	/* 打印文件大小和 inode 编号 */
	printf("file size: %ld bytes\n"
			"inode number: %ld\n", file_stat.st_size,
			file_stat.st_ino);
	exit(0);
}
