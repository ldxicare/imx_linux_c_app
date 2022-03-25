#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define IPV4_ADDR "192.168.0.66"
int main(void) {
	struct in_addr addr;
	char buf[20] = {0};
	inet_pton(AF_INET,IPV4_ADDR,&addr);
	printf("ip addr = %#.8x\n",addr.s_addr);
	inet_ntop(AF_INET, &addr, buf, sizeof(buf));
	printf("ip addr: %s\n", buf);
	exit(0);
}
