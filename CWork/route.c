#include "route.h"

int getRoute(char *requestHeader)
{
	/*
	desc -- 从Request中获取请求的页面代码
	Arguments -- Request字符串
	*/
	char *start = strchr(requestHeader, ' ');
	char *end = strchr(requestHeader + (start - requestHeader + 1), ' ');

	char url[48] = { 0 };
	strncpy(url, start + 1, end - start - 1);

	if ((stricmp("/", url)) == 0)
	{
		return INDEX;
	}
	else if (stricmp("/searchWeather", url) == 0)
	{
		return SEARCH_WEATHER;
	}
}