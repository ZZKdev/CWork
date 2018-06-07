#include "route.h"
#include "view.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>



View viewRoute(Request request)
{
	/*
		desc -- 根据请求路由
		Arguments -- 请求
	*/
	char path[256] = { 0 };
	getPath(request, path);

	View view = (View)malloc(8192);
	memset(view, 0, 8192);
	setResponseHeader(view);

	int i;
	UrlPattern urlPatterns[] = {
		{"/searchWeather", weatherView},
		{"/predictWeather", weather_predictView},
		{"/newpost", saveView},
		{"/showpost", showView},
		{"/deletepost", deleteView},
		{ "/", indexView }
	};
	for (i = 0; i < sizeof(urlPatterns) / sizeof(UrlPattern); i++)
	{
		if (strncmp(urlPatterns[i].url, path, strlen(urlPatterns[i].url) == 1 ? 2 : strlen(urlPatterns[i].url)) == 0)
		{
			return urlPatterns[i].view(view, request);
		}
	}
	return strcat(view, "<h1>404</h1>");

}

