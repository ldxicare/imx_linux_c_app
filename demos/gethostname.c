#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include <time.h>
int main(void) {

	struct tm t;
	time_t sec;
	struct utsname os_info;
	struct sysinfo sys_info;
	char hostname[20];
	int ret;
	memset(hostname, 0x0, sizeof(hostname));
	ret = gethostname(hostname, sizeof(hostname));
	if (-1 == ret) {
		perror("gethostname error");
		exit(ret);
	}
	puts(hostname);
	/* 获取信息 */
	ret = sysinfo(&sys_info);
	if (-1 == ret) {
		perror("sysinfo error");
		exit(-1);
	}
	/* 打印信息 */
	printf("uptime: %ld\n", sys_info.uptime);
	printf("totalram: %lu\n", sys_info.totalram);
	printf("freeram: %lu\n", sys_info.freeram);
	printf("procs: %u\n", sys_info.procs);

	/* 获取信息 */
	ret = uname(&os_info);
	if (-1 == ret) {
		perror("uname error");
		exit(-1);
	}
	/* 打印信息 */
	printf("操作系统名称: %s\n", os_info.sysname);
	printf("主机名: %s\n", os_info.nodename);
	printf("内核版本: %s\n", os_info.release);
	printf("发行版本: %s\n", os_info.version);
	printf("硬件架构: %s\n", os_info.machine);
	printf("每个用户的最大并发进程数: %ld\n", sysconf(_SC_CHILD_MAX));
	printf("系统节拍率: %ld\n", sysconf(_SC_CLK_TCK));
	printf("系统页大小: %ld\n", sysconf(_SC_PAGESIZE));
	/* 获取时间 */
	sec = time(NULL);
	if (-1 == sec) {
		perror("time error");
		exit(-1);
	}
	/* 转换得到本地时间 */
	localtime_r(&sec, &t);
	/* 打印输出 */
	printf("当前时间: %d 年%d 月%d 日 %d:%d:%d\n",
			t.tm_year + 1900, t.tm_mon, t.tm_mday,
			t.tm_hour, t.tm_min, t.tm_sec);
	exit(0);
}
