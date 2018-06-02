#include <WinSock2.h>
#include <stdio.h>
#include "controllers.h"
#include "route.h"
#include "view.h"
#include "server.h"

int loadConfig()
{
	setUp();
	int servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(80);
	sockAddr.sin_family = PF_INET;
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	listen(servSock, 20);
	return servSock;
}



void serverRun(int servSock)
{
	SOCKADDR clientAddr;
	int iSize = sizeof(SOCKADDR);
	char requestHeader[1024] = { 0 };
	while (1)
	{
		SetConsoleOutputCP(65001);
		int clientsock = accept(servSock, (SOCKADDR*)&clientAddr, &iSize);
		recv(clientsock, requestHeader, 1024, 0);
		log("%s",requestHeader);
		char path[256] = { 0 };
		getPath(requestHeader, path);
		View responseView = viewRoute(path);
		printf("%d", strlen(responseView));
		send(clientsock, responseView, strlen(responseView), 0);
	
		/*send(clientsock, "testsetestsetsetsetsetsetse", 20, 0);*/
		shutdown(clientsock, SD_SEND);
		printf(responseView);
		free(responseView);
		closesocket(clientsock);
	}
}
