#include <stdio.h>
#include <WinSock2.h>
#include "server.h"
#include "view.h"


int main(void)
{
	int servSock = loadConfig();
	
	serverRun(servSock);

	closesocket(servSock);
	WSACleanup();
	return 0;
}