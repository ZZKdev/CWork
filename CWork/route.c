#include "route.h"
#include "view.h"
#include "controllers.h"
#include <stdlib.h>
#include <string.h>

void getPath(char *requestHeader, char *path)
{
	/*
		desc -- 从请求头中获取请求路径
		Arguments -- 请求头和返回路径
	*/

	char *start = strchr(requestHeader, ' ');
	char *end = strchr(requestHeader + (start - requestHeader + 1), ' ');

	strncpy(path, start + 1, end - start - 1);
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
		deBug("len : %d", strlen(start));
		return strcpy(argument, start + 1);
	}
	return NULL;
}

View viewRoute(char *path)
{
	/*
		desc -- 根据路径选择路由
		Arguments -- 请求路径
	*/
	View view = (View)malloc(2048);
	memset(view, 0, 2048);
	setResponseHeader(view);
	char *argument = getArgument(path);
	printf("argument\n%s\n\n", argument);
	if ((stricmp(path, "/")) == 0)
	{		
		return indexView(view);
	}
	else if (strstr(path, "/searchWeather") != NULL)
	{
		return weatherView(view, argument);
	}

	return strcat(view, "<h1>404</h1>");
}