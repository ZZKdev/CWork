#include "view.h"
#include "controllers.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

View indexView(View view, Request request)
{
	char* bufferFile = readEntireFile("index.html");
	if (bufferFile == NULL)
	{
		fprintf(stderr, "open index.html error!\n");
		return strcat(view, u8"<h1>404</h1><p>服务器目录下无index页面</p>");
	}
	strcat(view, bufferFile);
	
	free(bufferFile);
	return view;
}

View weatherView(View view, Request request)
{
	char path[256] = { 0 };
	getPath(request, path);
	char* address = getArgument(path);
	if (address == NULL)
	{
		return strcat(view, "no address");
	}

	weatherInfo* weather = searchWeather(address);
	free(address);

	sprintf(view + strlen(view), u8"地点：%s</br>", weather->city);
	sprintf(view + strlen(view), u8"天气：%s</br>", weather->weather);
	sprintf(view + strlen(view), u8"风向：%s</br>", weather->winddirection);
	sprintf(view + strlen(view), u8"风力等级：%s</br>", weather->windpower);
	sprintf(view + strlen(view), u8"空气湿度：%s</br>", weather->humidity);
	sprintf(view + strlen(view), u8"温度：%s</br>", weather->temperature);
	sprintf(view + strlen(view), u8"报道时间：%s</br>", weather->reporttime);

	free(weather);
	return view;
}

View weather_predictView(View view, Request request)
{
	int i;
	char path[256] = { 0 };
	getPath(request, path);
	char* address = getArgument(path);
	if (address == NULL)
	{
		return strcat(view, "no address");
	}
	weather_predictInfo* weather = predictWeather(address);
	free(address);

	sprintf(view + strlen(view), u8"地点：%s</br>", weather->city);
	sprintf(view + strlen(view), u8"报道时间：%s</br>", weather->reporttime);

	for (i = 0; i <= 2; i++)
	{
		sprintf(view + strlen(view), u8"未来第%d天气情况</br>", i + 1);
		sprintf(view + strlen(view), u8"早上天气：%s</br>", weather->dayweather[i]);
		sprintf(view + strlen(view), u8"早上气温：%s</br>", weather->daytemp[i]);
		sprintf(view + strlen(view), u8"早上风向：%s</br>", weather->daywind[i]);
		sprintf(view + strlen(view), u8"早上风力等级：%s</br>", weather->daypower[i]);
		sprintf(view + strlen(view), u8"晚上天气：%s</br>", weather->nightweather[i]);
		sprintf(view + strlen(view), u8"晚上气温：%s</br>", weather->nighttemp[i]);
		sprintf(view + strlen(view), u8"晚上风向：%s</br>", weather->nightwind[i]);
		sprintf(view + strlen(view), u8"晚上风力等级：%s</br></br>", weather->nightpower[i]);

	}
	free(weather);
	return view;
}

View saveView(View view, Request request)
{
	char post[4096] = { 0 };
	fetchContent(request, post);
	decode(post);
	sprintf(post + strlen(post), "&id=%d\r\n\0", (get_maxId() + 1));
	
	
	
	FILE* database = fopen("database", "ab+");

	fwrite(post, 1, strlen(post), database);
	
	fclose(database);

	return strcat(view, u8"<h2>保存成功</h2>");
}

View showView(View view, Request request)
{
	linedList* phead = create_linedList();
	phead = sort_linedList(phead);
	linedList* pnode = phead;
	if (pnode == NULL)
	{
		return strcat(view, u8"<h2>暂无信息</h2>");
	}

	while(pnode)
	{
		sprintf(view + strlen(view), u8"地点：%s</br>记录：%s</br>", pnode->address, pnode->content);
		pnode = pnode->next;
	}
	free_linedList(phead);
	return view;
}

View deleteView(View view, Request request)
{
	char content[4096] = { 0 };
	fetchContent(request, content);
	decode(content);

	char address[128] = { 0 };
	sscanf(content, "%*[^=]=%[^&]", address);

	linedList* phead = create_linedList();
	phead = delete_node(phead, address);
	save_linedList(phead);

	return strcat(view, u8"<h1>删除成功</h1>");
}

View searchView(View view, Request request)
{
	char content[128] = { 0 };
	fetchContent(request, content);
	
	char address[128] = { 0 };
	sscanf(content, "%*[^=]=%[^&]", address);
	decode(address);

	int isSearch = 0;
	linedList* phead = create_linedList();
	linedList* pnode = phead;
	while (pnode)
	{
		if (strcmp(pnode->address, address) == 0)
		{
			isSearch = 1;
			sprintf(view + strlen(view), u8"<form action=\"/editpost\" method=\"post\">地点：%s<br />记录：<textarea cols=\"87\" rows=\"5\" name=\"content\">%s</textarea><br /><input type=\"text\" name=\"id\" value=\"%s\" style=\"display:none\"/><input type=\"submit\" value=\"修改\" style=\"float : right\" /></form>", pnode->address, pnode->content, pnode->id);
		}
		pnode = pnode->next;
	}
	if (isSearch == 0)
	{
		strcat(view, u8"<h1>无此地点记录</h1>");
	}
	free_linedList(phead);
	return view;
}

View editView(View view, Request request)
{
	char content[1024] = { 0 };
	fetchContent(request, content);
	decode(content);
	char id[32] = { 0 };
	char postContent[1024] = { 0 };
	sscanf(content, "%*[^=]=%[^&]", postContent);
	sscanf(content, "%*[^&]&%*[^=]=%[^&]", id);

	linedList* phead = create_linedList();
	linedList* pnode = phead;

	printf("\nid=%s\n", id);

	while (pnode)
	{
		printf("pnode->id = %s\n", pnode->id);
		printf("result=%d\n", strcmp(pnode->id, id));
		if (strcmp(pnode->id, id) == 0)
		{
			strcpy(pnode->content, postContent);
			break;
		}
		pnode = pnode->next;
	}
	save_linedList(phead);
	free_linedList(phead);
	return strcat(view, u8"<h1>修改成功</h1>");
}