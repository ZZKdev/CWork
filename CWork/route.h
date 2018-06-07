#pragma once

#define View char*
#define Request char*

typedef struct UrlPattern {
	char* url;
	View (*view)(View view, Request request);
}UrlPattern;

View viewRoute(Request request);
