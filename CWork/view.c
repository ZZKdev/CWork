#include "view.h"
#include "controllers.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

View indexView(View view, Request request)
{
	FILE* indexHtml = NULL;
	
	if ((indexHtml = fopen("index.html", "rb")) == NULL)
	{
		fprintf(stderr, "open index.html error!\n");
		return strcat(view, "<h1>couln't open index.html file</h1>");
	}
	char bufferFile[4096] = { 0 };
	fread(bufferFile, 1, 2779, indexHtml);
	strcat(view, bufferFile);
	fclose(indexHtml);
	
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
	strcat(post, SEPARATOR);
	
	
	FILE* database = fopen("database", "ab+");

	fwrite(post, 1, strlen(post), database);
	
	fclose(database);

	return strcat(view, u8"<h2>保存成功</h2>");
}

View showView(View view, Request request)
{
	linedList* pnode = create_linedList();
	if (pnode == NULL)
	{
		return strcat(view, u8"<h2>暂无信息</h2>");
	}
	while(pnode)
	{
		strcat(view, u8"地点：");
		strcat(view, pnode->address);
		strcat(view, "</br>");
		strcat(view, u8"记录：");
		strcat(view, pnode->content);
		strcat(view, "</br>");
		pnode = pnode->next;
	}
	free_linedList(pnode);
	return view;
}

View deleteView(View view, Request request)
{
	char content[4096] = { 0 };
	fetchContent(request, content);
	printf("\n\ncontent:\n%s", content);
	decode(content);
	printf("\ndecodecontent:%s\n",content);
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

	int flag = 0;
	linedList* phead = create_linedList();
	linedList* pnode = phead;
	while (pnode)
	{
		if (strcmp(pnode->address, address) == 0)
		{
			flag = 1;
			sprintf(view + strlen(view), u8"地点：%s</br>记录：%s</br>", pnode->address, pnode->content);
		}
		pnode = pnode->next;
	}
	if (flag == 0)
	{
		strcat(view, u8"<h1>无此地点记录</h1>");
	}
	free_linedList(phead);
	return view;
}