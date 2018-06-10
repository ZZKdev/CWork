#ifndef _OPERATION_H_
#define _OPERATION_H_
#include "models.h"

#define CONNECT_ERROR -1
#define REQUEST_START_NUMBER 4
#define REQUEST_END_NUMBER 218
#define SEARCH_ADCODE_STRING "/v3/place/text?offset=1&page=1&key=c5120cdde367302714edd8b76712fe07&extensions=all&keywords="
#define SEARCH_WEATHER_STRING "/v3/weather/weatherInfo?key=c5120cdde367302714edd8b76712fe07&city="
#define PREDICT_WEATHER_STRING "/v3/weather/weatherInfo?key=c5120cdde367302714edd8b76712fe07&extensions=all&city="


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
linedList* sort_linedList(linedList* phead);
int get_maxId();
#endif
