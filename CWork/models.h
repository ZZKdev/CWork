#pragma once
#define View char*
#define Request char*
#define SEPARATOR "\r\n"

typedef struct UrlPattern {
	char* url;
	View(*view)(View view, Request request);
}UrlPattern;

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
	int id;
	struct linedList* next;
}linedList;
