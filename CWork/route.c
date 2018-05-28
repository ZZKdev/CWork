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



View viewRoute(char *path)
{
	/*
		desc -- 根据路径选择路由
		Arguments -- 请求路径
	*/
	View view = (View)malloc(2048);
	memset(view, 0, 2048);
	setResponseHeader(view);

	if ((stricmp("/", path)) == 0)
	{		
		return indexView(view);
	}
	else if (stricmp("/searchWeather", path) == 0)
	{
		printf("fdsaf");
		return indexView(view);
	}
	printf("ccccc");
	return view;
}