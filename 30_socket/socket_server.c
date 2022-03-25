/***************************************************************
  Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
  文件名 : socket_server.c
  作者 : 邓涛
  版本 : V1.0
  描述 : 一个简单地TCP服务器应用程序示例代码
  其他 : 无
  论坛 : www.openedv.com
  日志 : 初版 V1.0 2021/7/20 邓涛创建
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

int listen_num = 0;
int listen_exit = 0;
#define SERVER_PORT     8888    //端口号不能发生冲突,不常用的端口号通常大于5000

static void *socket_handler(void *arg) 
{
	char recvbuf[256];
	int ret;
	int connfd  =*(int*)arg;

	/* 自行分离 */
	ret = pthread_detach(pthread_self());
	if (ret) {
		fprintf(stderr, "pthread_detach error: %s\n", strerror(ret));
		return NULL;
	}

	printf("新线程: 进程 ID<%d> 线程 ID<%lu>\n", getpid(), pthread_self());

	// 接收缓冲区清零
	memset(recvbuf, 0x0, sizeof(recvbuf));

	while(1){
		// 读数据
		ret = recv(connfd, recvbuf, sizeof(recvbuf)-1, 0);
		if(0 >= ret) {
			perror("recv error");
			close(connfd);
			break;
		}

		recvbuf[ret] = '\0';
		// 将读取到的数据以字符串形式打印出来
		printf("from client %d: %s",connfd, recvbuf);

		// 如果读取到"exit"则关闭套接字退出程序
		if (0 == strncmp("exit", recvbuf, 4)) {
			printf("server exit...\n");
			close(connfd);
			break;
		}
	}
	listen_exit++;
	return (void *)0; 
}

int main(void)
{
	struct sockaddr_in server_addr = {0};
	struct sockaddr_in client_addr = {0};
	char ip_str[20] = {0};
	int sockfd, connfd;
	int addrlen = sizeof(client_addr);
	int ret;
	pthread_t tid;

	/* 打开套接字，得到套接字描述符 */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > sockfd) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	/* 将套接字与指定端口号进行绑定 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);

	ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (0 > ret) {
		perror("bind error");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/* 使服务器进入监听状态 */
	ret = listen(sockfd, 32);
	if (0 > ret) {
		perror("listen error");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/* 接收客户端发送过来的数据 */
	for ( ; ; ) {

		/* 阻塞等待客户端连接 */
		connfd = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen);
		if (0 > connfd) {
			perror("accept error");
			close(sockfd);
			exit(EXIT_FAILURE);
		}

		printf("有客户端接入...\n");
		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip_str, sizeof(ip_str));
		printf("客户端主机的IP地址: %s\n", ip_str);
		printf("客户端进程的端口号: %d\n", client_addr.sin_port);
		

		/*create thread for handle socket*/
		ret = pthread_create(&tid, NULL, socket_handler, &connfd);
		if (ret) {
			fprintf(stderr, "Error: %s\n", strerror(ret));
			exit(-1);
		}
		if(listen_num == listen_exit && (listen_num != 0)){
			break;
		}
		listen_num++;
    }
	
	printf("main exit!\n");
    /* 关闭套接字 */
    close(sockfd);
    exit(EXIT_SUCCESS);
}
