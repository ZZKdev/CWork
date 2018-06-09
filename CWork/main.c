#include <stdio.h>
#include <WinSock2.h>
#include "server.h"


int main(void)
{
	printf("正在初始化服务器\n");
	int servSock = initServer();
	
	printf("服务器运行中.....\n");
	serverRun(servSock);

	closesocket(servSock);
	WSACleanup();
	return 0;
}