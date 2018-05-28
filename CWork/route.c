#include "route.h"
#include "view.h"
#include "controllers.h"
#include <stdlib.h>
#include <string.h>

void getPath(char *requestHeader, char *path)
{
	/*
		desc -- ������ͷ�л�ȡ����·��
		Arguments -- ����ͷ�ͷ���·��
	*/

	char *start = strchr(requestHeader, ' ');
	char *end = strchr(requestHeader + (start - requestHeader + 1), ' ');

	strncpy(path, start + 1, end - start - 1);
}

char* getArgument(char* path)
{
	/*
		desc -- ������·���л�ȡ����
		Arguments -- ����·��
		returns -- �������
	*/
	char* start = strchr(path, '=');
	if (start != NULL)
	{
		char *argument = malloc(strlen(start));
		return strcpy(argument, start + 1);
	}
	return NULL;
}

View viewRoute(char *path)
{
	/*
		desc -- ����·��ѡ��·��
		Arguments -- ����·��
	*/
	View view = (View)malloc(2048);
	memset(view, 0, 2048);
	setResponseHeader(view);
	char *argument = getArgument(path);
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