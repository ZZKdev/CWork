#pragma once

#define View char*
#define Request char*

typedef struct urlPattern {
	char* url;
	View (*view)(View view, Request request);
}urlPattern;

void getPath(char *requestHeader, char *path);
View viewRoute(char *request);
char* getArgument(char* path);
