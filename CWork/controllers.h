#ifndef _OPERATION_H_
#define _OPERATION_H_

#pragma warning(disable:4996)
#define CONNECT_ERROR -1
#define REQUEST_START_NUMBER 4
#define REQUEST_END_NUMBER 218
#define deBug printf
#define SEARCH_ADCODE "/v3/place/text?offset=1&page=1&key=c5120cdde367302714edd8b76712fe07&extensions=all&keywords="
#define SEARCH_WEATHER_STRING "/v3/weather/weatherInfo?key=c5120cdde367302714edd8b76712fe07&city="


typedef struct weatherInfo {
	char city[24];
	char weather[24];
	int temperature;
	char winddirection[24];
	int windpower;
	int humidity;
	char reporttime[32];
}weatherInfo;

void setUp();
int linkTarget(char* ip, short port);
char* sendRequest(int sock, char* url);
void makeRequest(char* url, char* request);
void fetchJsonString( char *response, char *jsonString);
void fetchAdcode(char *jsonString, char *adcode);
void getAdcode(char *address, char *adcode);
void fetchWeatherInfo(char *jsonString, weatherInfo* weather);
weatherInfo* searchWeather(char *address);
void setResponseHeader(char* response);
#endif
