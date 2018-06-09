#include <WinSock2.h>
#include <stdio.h>
#include "controllers.h"
#include "route.h"
#include "view.h"
#include "server.h"

cJSON* loadConfig()
{
	/*
		desc -- 加载配置文件并返回json对象
		returns -- 含文件配置信息的json对象
	*/
	char* buffer = readEntireFile("config");

	cJSON* root = cJSON_Parse(buffer);
	free(buffer);
	return root;
}

int initServer()
{
	setUp();
	cJSON* root = loadConfig();
	
	char* ip = cJSON_GetObjectItem(root, "ip")->valuestring;
	int port = cJSON_GetObjectItem(root, "port")->valueint;
	
	int servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_addr.s_addr = inet_addr(ip);
	sockAddr.sin_port = htons(port);
	sockAddr.sin_family = PF_INET;
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	
	cJSON_Delete(root);
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
		printf("正在处理请求...\n");
		printf(request);
		View responseView = viewRoute(request);
		send(clientsock, responseView, strlen(responseView), 0);
		memset(request, 0, sizeof(request));
	
		shutdown(clientsock, SD_SEND);
		free(responseView);
		closesocket(clientsock);
		printf("处理完成，等待下一个请求...\n");
	}
}
