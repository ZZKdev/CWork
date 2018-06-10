#include <stdio.h>
#include "controllers.h"
#include <WinSock2.h>
#include "cJSON.h"

/********************网络层************************/
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

	if (connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) != 0)
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
	char request[1024] = { 0 };
	makeRequest(url, request);

	char* response = (char*)malloc(4096);
	memset(response, 0, 4096);

	send(sock, request, strlen(request), 0);
	shutdown(sock, SD_SEND);


	int n = recv(sock, response, 4096, MSG_WAITALL);
	return response;
}



/**********************字符串处理层***************************/
void getPath(Request request, char *path)
{
	/*
		desc -- 从请求中获取请求路径
		Arguments -- 请求和返回路径
	*/

	sscanf(request, "%*[^ ] %[^ ]", path);
}

char* getArgument(char* path)
{
	/*
		desc -- 从请求路径中获取参数
		Arguments -- 请求路径
		returns -- 请求参数
	*/
	char* start = strchr(path, '=');
	if (start != NULL)
	{
		char *argument = malloc(strlen(start));
		return strcpy(argument, start + 1);
	}
	return NULL;
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

void makeRequest(char* url, char* request)
{
	/*
		desc -- 根据指定URL制作一个请求头
		Arguments -- URL和请求头的指针
	*/
	FILE* requestFile = NULL;
	if ((requestFile = fopen("request", "rb")) == NULL)
	{
		fprintf(stderr, "open requestFile error!\n");
		return;
	}

	fread(request, 1, REQUEST_START_NUMBER, requestFile);
	strcat(request, url);
	fread(&request[strlen(request)], 1, REQUEST_END_NUMBER, requestFile);

	fclose(requestFile);
}

int hexToDec(char c)
{
	/*
		desc -- 将16进制数转成10进制数
		arguments -- 要转换的16进制数
		returns -- 返回转换完成的10进制数
	*/
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
	/*
		desc -- 将url编码的字符串解码
		arguments -- 要转化的目标字符串
	*/
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
			num = hexToDec(c1) * 16 + hexToDec(c0);
			result[res_len++] = num;
		}
	}
	result[res_len] = '\0';
	strcpy(destination, result);
	free(result);
}

void fetchContent(char *response_or_request, char *content)
{
	/*
		desc -- 从整个响应或请求中获取其中的响应内容或请求内容
		Arguments -- 响应或请求、返回的内容
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
	strcpy(weather->humidity, cJSON_GetObjectItem(jsonLiveItem, "humidity")->valuestring);
	strcpy(weather->temperature, cJSON_GetObjectItem(jsonLiveItem, "temperature")->valuestring);
	strcpy(weather->windpower, cJSON_GetObjectItem(jsonLiveItem, "windpower")->valuestring);

}

void fetchWeather_predictInfo(char *jsonString, weather_predictInfo* weather)
{
	/*
		desc -- 从json字符串中提取预测的天气信息
		Arguments -- json字符串和返回的结构体
	*/
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

char* readEntireFile(const char *fileName)
{
	/*
		desc -- 读取整个文件并返回字符串
		Arguments -- 文件名
		returns -- 读取完的字符串
	*/
	FILE* file = fopen(fileName, "rb");

	fseek(file, 0, SEEK_END);
	long fileLength = ftell(file);
	char* buffer = (char*)malloc(fileLength + 1);

	if (buffer == NULL)
	{
		return NULL;
	}

	fseek(file, 0, SEEK_SET);
	fread(buffer, fileLength, 1, file);
	buffer[fileLength] = '\0';

	fclose(file);
	return buffer;
}

/*************************链表操作层*********************************/

linedList* create_linedList()
{
	/*
		desc -- 读取数据库文件建立链表
		returns -- 返回头节点
	*/
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
			memset(phead, 0, sizeof(linedList));
			pnode = phead;
		}
		else
		{
			pnode->next = malloc(sizeof(linedList));
			memset(pnode->next, 0, sizeof(linedList));
			pnode = pnode->next;
		}
		pnode->next = NULL;
		
		sscanf(buffer, "%*[^=]=%[^&]", pnode->address);
		sscanf(buffer, "%*[^&]&%*[^=]=%[^&]", pnode->content);
	}
	fclose(database);
	return phead;
}

void free_linedList(linedList* pnode)
{
	/*
		desc -- 释放链表
		arguments -- 头节点
	*/
	linedList* ptemp;
	while (pnode)
	{
		ptemp = pnode->next;
		free(pnode);
		pnode = ptemp;
	}
}

linedList* delete_node(linedList* phead, char *address)
{
	/*
		desc -- 删除目标地点记录
		arguments -- 头节点和目标地点
		returns -- 头节点
	*/
	linedList* pnode = phead;
	linedList* ptemp = NULL;
	printf("\nplen:%d", strlen(phead->address));
	printf("\nlen:%d", strlen(address));
	printf("\npaddress:%s\n", phead->address);
	printf("address:%s\n", address);
	while (pnode)
	{
		if (pnode == phead && strcmp(phead->address, address) == 0)
		{
			phead = phead->next;
			free(pnode);
			pnode = phead;
			continue;
		}
		else if (pnode->next != NULL && strcmp(pnode->next->address, address) == 0)
		{
			ptemp = pnode->next->next;
			free(pnode->next);
			pnode->next = ptemp;
			continue;
		}
		pnode = pnode->next;
	}
	return phead;
}

void save_linedList(linedList* pnode)
{
	/*
		desc -- 保存链表信息到数据库
		arguments -- 头节点
	*/
	FILE* database = fopen("database", "wb");
	while (pnode)
	{
		fprintf(database, "address=%s", pnode->address);
		fprintf(database, "&content=%s", pnode->content);
		pnode = pnode->next;
	}
	fclose(database);
}

//linedList* sort_linedList(linedList* phead)
//{
//	linedList* pnode[5] = { NULL, NULL, NULL, NULL, NULL };
//
//	for (pnode[0] = phead; pnode[0]; pnode[0] = pnode[0]->next)
//	{
//		for (pnode[1] = pnode[0]->next; pnode[1]; pnode[1] = pnode[1]->next)
//		{
//			if (strcmp(pnode[0]->address, pnode[1]->address) < 0)
//			{
//				if (pnode[0] == phead && pnode[3] == NULL)
//				{
//					phead = pnode[1];
//					pnode[2] = pnode[1]->next;
//					pnode[1]->next = pnode[0];
//					pnode[0]->next = pnode[2];
//				}
//				else if(pnode[0] == phead)
//				{
//					phead = pnode[1];
//					pnode[2] = pnode[1]->next;
//					pnode[1]->next = pnode[0]->next;
//					pnode[0]->next = pnode[2];
//					pnode[3]->next = pnode[0];
//				}
//				else if (pnode[3] == NULL)
//				{
//					pnode[2] = pnode[1]->next;
//					pnode[1]->next = pnode[0];
//					pnode[0]->next = pnode[2];
//					pnode[4]->next = pnode[1];
//				}
//				else
//				{
//					pnode[2] = pnode[1]->next;
//					pnode[1]->next = pnode[0]->next;
//					pnode[0]->next = pnode[2];
//					pnode[3]->next = pnode[0];
//					pnode[4]->next = pnode[1];
//				}
//				pnode[2] = pnode[1];
//				pnode[1] = pnode[0];
//				pnode[0] = pnode[2];
//			}
//			pnode[3] = pnode[1];
//		}
//		pnode[3] = NULL;
//		pnode[4] = pnode[0];
//	}
//	return phead;
//}
linedList* sort_linedList(linedList* phead)
{
	/*
		desc -- 对链表进行排序，将名字相同的排在一起
		Arguments -- 头节点
		returns -- 头节点
	*/
	linedList* pnode[5] = { NULL, NULL, NULL, NULL, NULL };

	for (pnode[0] = phead; pnode[0]; pnode[0] = pnode[0]->next)
	{
		for (pnode[1] = pnode[0]->next; pnode[1]; pnode[1] = pnode[1]->next)
		{
			if (strcmp(pnode[0]->address, pnode[1]->address) < 0)
			{
				pnode[2] = pnode[1]->next;
				if (pnode[0] == phead)
				{
					phead = pnode[1];
					if (pnode[3] == NULL)
					{
						pnode[1]->next = pnode[0];
					}
					else
					{
						pnode[1]->next = pnode[0]->next;
						pnode[3]->next = pnode[0];
					}
				}
				else
				{
					if (pnode[3] == NULL)
					{
						pnode[1]->next = pnode[0];
					}
					else
					{
						pnode[1]->next = pnode[0]->next;
						pnode[3]->next = pnode[0];
					}	
					pnode[4]->next = pnode[1];
				}
				pnode[0]->next = pnode[2];

				pnode[2] = pnode[1];
				pnode[1] = pnode[0];
				pnode[0] = pnode[2];
			}
			pnode[3] = pnode[1];
		}
		pnode[3] = NULL;
		pnode[4] = pnode[0];
	}
	return phead;
}


/***************************应用层*****************************/

void getAdcode(char *address,char *adcode)
{
	/*
		desc -- 获取指定地址的adcode
		Arguments -- 目标地址和返回的adcode
	*/
	int sock = linkTarget("106.11.12.1", 80);
	char url[256] = { 0 };
	strcat(url, SEARCH_ADCODE_STRING);
	strcat(url, address);

	char* response = sendRequest(sock, url);
	char jsonString[4096] = { 0 };
	

	fetchContent(response, jsonString);

	fetchAdcode(jsonString, adcode);

	free(response);
	closesocket(sock);
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

weather_predictInfo* predictWeather(char* address)
{
	/*
		desc -- 查询地址的未来三天的天气信息
		Arguments -- 请求地址
		returns -- 地址信息结构体
	*/
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



