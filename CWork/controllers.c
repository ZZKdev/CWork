#include <stdio.h>
#include "controllers.h"
#include <WinSock2.h>
#include "cJSON.h"


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
	char url[256] = { 0 };
	strcat(url, SEARCH_ADCODE);
	strcat(url, address);

	char* response = sendRequest(sock, url);
	char jsonString[4096] = { 0 };
	

	

	fetchContent(response, jsonString);

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

void fetchWeather_predictInfo(char *jsonString, weather_predictInfo* weather)
{
	cJSON* jsonRoot = cJSON_Parse(jsonString);
	cJSON* jsonForecasts = cJSON_GetObjectItem(jsonRoot, "forecasts");
	cJSON* jsonForecastItem = cJSON_GetArrayItem(jsonForecasts, 0);

	strcpy(weather->city, cJSON_GetObjectItem(jsonForecastItem, "city")->valuestring);
	strcpy(weather->reporttime, cJSON_GetObjectItem(jsonForecastItem, "reporttime")->valuestring);
	cJSON* jsonCasts = cJSON_GetObjectItem(jsonForecastItem, "casts");
	int i = 1;
	for (int i = 1; i <= 3; i++)
	{
		cJSON* jsonCastsItem = cJSON_GetArrayItem(jsonCasts, i);
		strcpy(weather->daypower[i - 1], cJSON_GetObjectItem(jsonCastsItem, "daypower")->valuestring);
		strcpy(weather->daytemp[i - 1], cJSON_GetObjectItem(jsonCastsItem, "daytemp")->valuestring);
		strcpy(weather->dayweather[i - 1], cJSON_GetObjectItem(jsonCastsItem, "dayweather")->valuestring);
		strcpy(weather->daywind[i - 1], cJSON_GetObjectItem(jsonCastsItem, "daywind")->valuestring);
		strcpy(weather->nightpower[i - 1], cJSON_GetObjectItem(jsonCastsItem, "nightpower")->valuestring);
		strcpy(weather->nighttemp[i - 1], cJSON_GetObjectItem(jsonCastsItem, "nighttemp")->valuestring);
		strcpy(weather->nightweather[i - 1], cJSON_GetObjectItem(jsonCastsItem, "nightweather")->valuestring);
		strcpy(weather->nightwind[i - 1], cJSON_GetObjectItem(jsonCastsItem, "nightwind")->valuestring);
	}
}

weatherInfo* searchWeather(char *address)
{	
	/*
		desc -- 查询地址的当天的天气信息
		Arguments -- 请求地址
		returns -- 地址信息结构体
	*/
	char adcode[12] = { 0 };
	getAdcode(address, adcode);

	char url[256] = { 0 };
	strcat(url, SEARCH_WEATHER_STRING);
	strcat(url, adcode);

	printf(adcode);

	int sock = linkTarget("106.11.12.1", 80);
	char* response = sendRequest(sock, url);

	char jsonString[4096] = { 0 };
	fetchContent(response, jsonString);
	weatherInfo* weather = (weatherInfo *)malloc(sizeof(weatherInfo));
	fetchWeatherInfo(jsonString, weather);

	free(response);
	return weather;
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
    char request[1024] = {0};
    makeRequest(url, request);

    char* response = (char*) malloc(4096);
	memset(response, 0, 4096);

    send(sock, request, strlen(request), 0);
	shutdown(sock, SD_SEND);
	

    int n = recv(sock, response, 4096, MSG_WAITALL);
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

void fetchContent(char *response_or_request, char *content)
{
	/*
		desc -- 在response中抓取json字符串
		Arguments -- response和返回的json字符串
	*/
    strcpy(content, strstr(response_or_request, "\r\n\r\n") + 4);
}
void setResponseHeader(char* response)
{
	/*
		desc -- 给响应设置响应头
		Arguments -- 要设置响应头的响应
	*/
	char* Header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n";
	strcat(response, Header);
}

weather_predictInfo* predictWeather(char* address)
{
	weather_predictInfo* weather = (weather_predictInfo*)malloc(sizeof(weather_predictInfo));
	memset(weather, 0, sizeof(weather));

	char adcode[12] = { 0 };
	getAdcode(address, adcode);
	


	char url[256] = { 0 };
	strcat(url, PREDICT_WEATHER_STRING);
	strcat(url, adcode);

	int sock = linkTarget("106.11.12.1", 80);
	char* response = sendRequest(sock, url);

	char jsonString[4096] = { 0 };
	fetchContent(response, jsonString);


	fetchWeather_predictInfo(jsonString, weather);

	return weather;
}

int hex2dec(char c)
{
	if ('0' <= c && c <= '9')
	{
		return c - '0';
	}
	else if ('a' <= c && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else if ('A' <= c && c <= 'F')
	{
		return c - 'A' + 10;
	}
	else
	{
		return -1;
	}
}
void decode(char* destination)
{
	int i = 0;
	int len = strlen(destination);
	int res_len = 0;
	char num, c0, c1;
	char* result = malloc(len + 1);
	for (i = 0; i < len; ++i)
	{
		char c = destination[i];
		if (c != '%')
		{
			result[res_len++] = c;
		}
		else
		{
			c1 = destination[++i];
			c0 = destination[++i];
			num = hex2dec(c1) * 16 + hex2dec(c0);
			result[res_len++] = num;
		}
	}
	result[res_len] = '\0';
	strcpy(destination, result);
	free(result);
}

linedList* create_linedList()
{
	FILE* database = fopen("database", "rb");
	if (database == NULL)
	{
		return NULL;
	}
	char buffer[2048] = { 0 };
	char title[64] = { 0 };
	char content[1024] = { 0 };
	
	linedList* phead = NULL;
	linedList* pnode = NULL;

	while (fgets(buffer, 2048, database) != NULL)
	{
		if (phead == NULL)
		{
			phead = malloc(sizeof(linedList));
			pnode = phead;
		}
		else
		{
			pnode->next = malloc(sizeof(linedList));
			pnode = pnode->next;
		}
		pnode->next = NULL;
		sscanf(buffer, "%*[^=]=%[^&]", pnode->title);
		sscanf(buffer, "%*[^&]&%*[^=]=%[^&]", pnode->content);
	}
	fclose(database);
	return phead;
}

void free_linedList(linedList* pnode)
{
	linedList* ptemp;
	while (pnode)
	{
		ptemp = pnode->next;
		free(pnode);
		pnode = ptemp;
	}
}