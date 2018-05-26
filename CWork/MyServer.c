#include "operation.h"
#include <stdio.h>
#include <WinSock2.h>

int main(void)
{
	setUp();
	int servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(12345);
	sockAddr.sin_family = PF_INET;
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	listen(servSock, 20);
	
	SOCKADDR clientAddr;
	int iSize = sizeof(SOCKADDR);
	char* buffer = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>hello World</h1>";
	FILE* indexHtml = NULL;
	if ((indexHtml = fopen("index.html", "rb")) == NULL)
	{
		fprintf(stderr, "open requestFile error!\n");
		return;
	}
	char bufferHtml[4096] = { 0 };
	strcat(bufferHtml, buffer);
	fread(bufferHtml + strlen(buffer), 1, 236, indexHtml);
	fclose(indexHtml);
	
	while (1)
	{
		int clientsock = accept(servSock, (SOCKADDR*)&clientAddr, &iSize);
		printf("hello client");
		send(clientsock, bufferHtml, strlen(bufferHtml), 0);
		closesocket(clientsock);
	}
	
		
	
	

	closesocket(servSock);
	WSACleanup();
	return 0;
}