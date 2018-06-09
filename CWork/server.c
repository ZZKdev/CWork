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
	sockAddr.sin_addr.s_addr = inet_addr("172.24.81.114");
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
	char request[4096] = { 0 };
	while (1)
	{
		int clientsock = accept(servSock, (SOCKADDR*)&clientAddr, &iSize);
		recv(clientsock, request, 4096, 0);
		log("\nrecv:\n%s\n",request);
		View responseView = viewRoute(request);
		send(clientsock, responseView, strlen(responseView), 0);
		memset(request, 0, sizeof(request));
		/*send(clientsock, "testsetestsetsetsetsetsetse", 20, 0);*/
		shutdown(clientsock, SD_SEND);
		free(responseView);
		closesocket(clientsock);
	}
}
