#include "route.h"

int getRoute(char *requestHeader)
{
	/*
	desc -- ��Request�л�ȡ�����ҳ�����
	Arguments -- Request�ַ���
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