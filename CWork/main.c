#include <stdio.h>
#include <WinSock2.h>
#include "server.h"


int main(void)
{
	int servSock = loadConfig();
	SetConsoleOutputCP(65001);
	serverRun(servSock);

	closesocket(servSock);
	WSACleanup();
	return 0;
}