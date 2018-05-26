#include <stdio.h>
#include "operation.h"
#include <WinSock2.h>
#include "cJSON.h"

void helloView()
{
	printf("欢迎使用本系统！\n");
	printf("您可以使用以下功能\n");
	printf("1. 查询天气\n");
	printf("0. 退出系统\n");
}

void fetchAdcode(char *jsonString, char *adcode)
{
	/*
		desc -- 在json字符串中寻找adocde
		Arguments -- json字符串和返回的adcode
	*/
	cJSON* jsonRoot = cJSON_Parse(jsonString);
	cJSON* jsonPois = cJSON_GetObjectItem(jsonRoot, "pois");
	cJSON* jsonadcoe = cJSON_GetObjectItem(cJSON_GetArrayItem(jsonPois, 0), "adcode");
	strcpy(adcode, jsonadcoe->valuestring);
	cJSON_Delete(jsonRoot);
}

void getAdcode(char *address,char *adcode)
{
	/*
		desc -- 获取指定地址的adcode
		Arguments -- 目标地址和返回的adcode
	*/
	int sock = linkTarget("106.11.12.1", 80);
	char url[128] = { 0 };
	strcat(url, SEARCH_ADCODE);
	strcat(url, address);

	char* response = sendRequest(sock, url);
	char jsonString[2048] = { 0 };
	fetchJsonString(response, jsonString);

	fetchAdcode(jsonString, adcode);

	free(response);
	closesocket(sock);
}

void fetchWeatherInfo(char *jsonString, weatherInfo* weather)
{
	/*
		desc -- 从json字符串中提取weather信息
		Arguments -- json字符串和接受天气信息的结构体
	*/
	cJSON* jsonRoot = cJSON_Parse(jsonString);
	cJSON* jsonLive = cJSON_GetObjectItem(jsonRoot, "lives");
	cJSON* jsonLiveItem = cJSON_GetArrayItem(jsonLive, 0);
	strcpy(weather->city, cJSON_GetObjectItem(jsonLiveItem, "city")->valuestring);
	strcpy(weather->reporttime, cJSON_GetObjectItem(jsonLiveItem, "reporttime")->valuestring);
	strcpy(weather->weather, cJSON_GetObjectItem(jsonLiveItem, "weather")->valuestring);
	strcpy(weather->winddirection, cJSON_GetObjectItem(jsonLiveItem, "winddirection")->valuestring);

	weather->humidity = atoi(cJSON_GetObjectItem(jsonLiveItem, "humidity")->valuestring);
	weather->temperature = atoi(cJSON_GetObjectItem(jsonLiveItem, "temperature")->valuestring);
	weather->windpower = atoi(cJSON_GetObjectItem(jsonLiveItem, "windpower")->valuestring);

}

void searchWeather()
{
	char address[52] = { 0 };
	printf("输入搜索地点:");
	scanf("%s", address);

	
	char adcode[12] = { 0 };
	getAdcode(address, adcode);
	
	char url[128] = { 0 };
	strcat(url, SEARCH_WEATHER_STRING);
	strcat(url, adcode);

	int sock = linkTarget("106.11.12.1", 80);
	char* response = sendRequest(sock, url);

	char jsonString[2048] = { 0 };
	fetchJsonString(response, jsonString);
	weatherInfo* weather = (weatherInfo *)malloc(sizeof(weatherInfo));
	fetchWeatherInfo(jsonString, weather);
	printf("%s", weather->city);
	
	free(response);
}

int main(void)
{
	setUp();
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	int flag = -1;
	helloView();
	while (flag)
	{
		printf("请输入指令：");
		scanf("%d", &flag);
		switch (flag)
		{
		case 1:
			searchWeather();
			break;
		case 0:
			break;
		default:
			printf("输入指令无效！\n");
			break;
		}
	}	
	return 0;
}    


void setUp()
{
    /*
        desc -- 加载并初始化dll
    */
    #pragma comment(lib, "ws2_32.lib")
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

int linkTarget(char *ip, short port)
{
    /* 
        desc -- 与目标主机建立连接
        Arguments -- ip和端口号
        returns -- 套接字
    */
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_port = htons(port);
    sockAddr.sin_addr.s_addr = inet_addr(ip);

    if(connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) != 0)
    {
        fprintf(stderr, "connect error!\n");
        return CONNECT_ERROR;
    }

    return sock;
}

char* sendRequest(int sock, char *url)
{
    /*
        desc -- 向指定URL发送请求
        Arguments -- 套接字和URL
        Returns -- 响应头
    */
    char request[512] = {0};
    makeRequest(url, request);

    char* response = (char*) malloc(2048);
	memset(response, 0, 2048);
    send(sock, request, strlen(request), 0);
    recv(sock, response, 2048, 0);

    return response;
}


void makeRequest(char* url, char* request)
{
    /* 
        desc -- 根据指定URL制作一个请求头
        Arguments -- URL和请求头的指针
    */
    FILE* requestFile = NULL;
    if((requestFile = fopen("request", "rb")) == NULL)
    {
        fprintf(stderr, "open requestFile error!\n");
        return;
    }

    fread(request, 1, REQUEST_START_NUMBER, requestFile);
    strcat(request, url);
    fread(&request[strlen(request)], 1, REQUEST_END_NUMBER, requestFile);
    fclose(requestFile);
}

void fetchJsonString( char *response, char *jsonString)
{
	/*
		desc -- 在response中抓取json字符串
		Arguments -- response和返回的json字符串
	*/
    strcpy(jsonString, strstr(response, "\r\n\r\n") + 4);
}
