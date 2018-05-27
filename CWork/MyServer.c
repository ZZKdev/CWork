#include "operation.h"
#include <stdio.h>
#include <WinSock2.h>
#define index 0
#define SEARCHWEATHER 1

int getRoute(char *bufferRecv)
{
	char *start = strchr(bufferRecv, ' ');
	char *end = strchr(bufferRecv + (start - bufferRecv + 1), ' ');
	
	char url[48] = { 0 };
	strncpy(url, start + 1, end - start - 1);

	if ((stricmp("/", url)) == 0)
	{
		return 0;
	}
	else if (stricmp("/searchWeather", url) == 0)
	{
		return 1;
	}
}

int main(void)
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
	
	SOCKADDR clientAddr;
	int iSize = sizeof(SOCKADDR);
	char* buffer = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n";
	FILE* indexHtml = NULL;
	if ((indexHtml = fopen("index.html", "rb")) == NULL)
	{
		fprintf(stderr, "open requestFile error!\n");
		return;
	}
	char bufferHtml[4096] = { 0 };
	char bufferRecv[1024] = { 0 };
	strcat(bufferHtml, buffer);
	fread(bufferHtml + strlen(buffer), 1, 428, indexHtml);
	fclose(indexHtml);
	
	while (1)
	{
		int clientsock = accept(servSock, (SOCKADDR*)&clientAddr, &iSize);
		printf("hello client");
		recv(clientsock, bufferRecv, 1024, 0);
		int route = getRoute(bufferRecv);
		switch (route)
		{
		case index:
			send(clientsock, bufferHtml, strlen(bufferHtml), 0);
			break;
		case SEARCHWEATHER:
			send(clientsock, bufferRecv, strlen(bufferRecv), 0);
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