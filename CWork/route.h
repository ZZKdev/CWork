#pragma once

#define View char*


void getPath(char *requestHeader, char *path);
View viewRoute(char *request);
char* getArgument(char* path);