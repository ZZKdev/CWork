#ifndef _OPERATION_H_
#define _OPERATION_H_

#pragma warning(disable:4996)
#define CONNECT_ERROR -1
#define REQUEST_START_NUMBER 4
#define REQUEST_END_NUMBER 218
#define SEARCH_ADCODE "/v3/place/text?offset=1&page=1&key=c5120cdde367302714edd8b76712fe07&extensions=all&keywords="
#define SEARCH_WEATHER_STRING "/v3/weather/weatherInfo?key=c5120cdde367302714edd8b76712fe07&city="
#define PREDICT_WEATHER_STRING "/v3/weather/weatherInfo?key=c5120cdde367302714edd8b76712fe07&extensions=all&city="
#define Request char*

typedef struct weatherInfo {
	char city[24];
	char weather[24];
	char temperature[4];
	char winddirection[24];
	char windpower[4];
	char humidity[4];
	char reporttime[32];
}weatherInfo;

typedef struct weather_predictInfo {
	char city[24];
	char dayweather[3][24];
	char nightweather[3][24];
	char daytemp[3][4];
	char nighttemp[3][4];
	char daywind[3][24];
	char nightwind[3][24];
	char daypower[3][12];
	char nightpower[3][12];
	char reporttime[32];
}weather_predictInfo;

typedef struct linedList {
	char address[64];
	char content[1024];
	struct linedList* next;
}linedList;



void setUp();
int linkTarget(char* ip, short port);
char* sendRequest(int sock, char* url);
void makeRequest(char* url, char* request);
void fetchContent(char *response_or_request, char *content);
void fetchAdcode(char *jsonString, char *adcode);
void getAdcode(char *address, char *adcode);
void fetchWeatherInfo(char *jsonString, weatherInfo* weather);
weatherInfo* searchWeather(char *address);
void setResponseHeader(char* response);
weather_predictInfo* predictWeather(char* address);
void decode(char* string);
linedList* create_linedList();
void free_linedList(linedList* pnode);
linedList* delete_node(linedList* phead, char *address);
void save_linedList(linedList* pnode);
void getPath(Request request, char *path);
char* getArgument(char* path);
char* readEntireFile(const char *fileName);
#endif
