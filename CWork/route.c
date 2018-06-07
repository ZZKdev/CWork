#include "route.h"
#include "view.h"
#include "controllers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void getPath(char *requestHeader, char *path)
{
	/*
		desc -- 从请求头中获取请求路径
		Arguments -- 请求头和返回路径
	*/

	sscanf(requestHeader, "%*[^ ] %[^ ]", path);
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

View viewRoute(char *request)
{
	/*
		desc -- 根据请求路由
		Arguments -- 请求
	*/
	char path[1024] = { 0 };
	getPath(request, path);

	View view = (View)malloc(8192);
	memset(view, 0, 8192);
	setResponseHeader(view);

	int i;
	urlPattern urlpatterns[] = {
		{"/searchWeather", weatherView},
		{"/predictWeather", weather_predictView},
		{"/newpost", saveView},
		{"/showpost", showView},
		{"/deletepost", deleteView},
		{ "/", indexView }
	};
	for (i = 0; i < sizeof(urlpatterns) / sizeof(urlPattern); i++)
	{
		if (strncmp(urlpatterns[i].url, path, strlen(urlpatterns[i].url) == 1 ? 2 : strlen(urlpatterns[i].url)) == 0)
		{
			return urlpatterns[i].view(view, request);
		}
	}
	return strcat(view, "<h1>404</h1>");

}