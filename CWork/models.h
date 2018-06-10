#pragma once
#define View char*
#define Request char*


typedef struct UrlPattern {
	char* url;									//URL地址
	View(*view)(View view, Request request);	//对于URL地址要调用的视图函数
}UrlPattern;

typedef struct weatherInfo {
	char city[24];								//城市
	char weather[24];							//天气
	char temperature[4];						//温度
	char winddirection[24];						//风向
	char windpower[4];							//风力等级
	char humidity[4];							//空气湿度
	char reporttime[32];						//报道时间
}weatherInfo;

typedef struct weather_predictInfo {
	char city[24];								//城市
	char dayweather[3][24];						//未来三天早上的天气
	char nightweather[3][24];					//未来三天晚上的天气
	char daytemp[3][4];							//未来三天早上的温度
	char nighttemp[3][4];						//未来三天晚上的温度
	char daywind[3][24];						//未来三天早上的风向
	char nightwind[3][24];						//未来三天晚上的风向
	char daypower[3][12];						//未来三天早上的风力等级
	char nightpower[3][12];						//未来三天晚上的风力等级
	char reporttime[32];						//报道时间
}weather_predictInfo;

typedef struct linedList {
	char address[64];							//地点
	char content[1024];							//记录内容
	char id[32];								//id号
	struct linedList* next;						//下一个节点的地址
}linedList;
