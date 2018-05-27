#include <WinSock2.h>


int serverRun()
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

void setResponseHeader(char* response)
{
	char* Header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n";
	strcat(response, Header);
}