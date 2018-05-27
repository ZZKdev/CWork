#include "operation.h"
#include <stdio.h>
#include <WinSock2.h>
#include "route.h"
#include "server.h"



int main(void)
{
	int servSock = serverRun();
	
	SOCKADDR clientAddr;
	int iSize = sizeof(SOCKADDR);
	FILE* indexHtml = NULL;
	if ((indexHtml = fopen("index.html", "rb")) == NULL)
	{
		fprintf(stderr, "open requestFile error!\n");
		return;
	}
	char bufferHtml[4096] = { 0 };
	setResponseHeader(bufferHtml);
	fread(bufferHtml + HEADER_LENGTH, 1, 428, indexHtml);
	fclose(indexHtml);
	
	char requestHeader[1024] = { 0 };

	while (1)
	{
		int clientsock = accept(servSock, (SOCKADDR*)&clientAddr, &iSize);
		printf("hello client");
		recv(clientsock, requestHeader, 1024, 0);
		int route = getRoute(requestHeader);
		switch (route)
		{
		case INDEX:
			send(clientsock, bufferHtml, strlen(bufferHtml), 0);
			break;
		case SEARCH_WEATHER:
			send(clientsock, requestHeader, strlen(requestHeader), 0);
			break;
		default:
			printf("fdsafd"); 
			break;
		}
		
		
		shutdown(clientsock, SD_SEND);
		closesocket(clientsock);
	}
	
	closesocket(servSock);
	WSACleanup();
	return 0;
}