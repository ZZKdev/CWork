#include <stdio.h>
#include <string.h>
#include "view.h"
#include "controllers.h"

View indexView(View view)
{
	FILE* indexHtml = NULL;
	
	if ((indexHtml = fopen("index.html", "rb")) == NULL)
	{
		fprintf(stderr, "open index.html error!\n");
		return strcat(view, "<h1>couln't open index.html file</h1>");
	}
	char bufferFile[4096] = { 0 };
	fread(bufferFile, 1, 2261, indexHtml);
	strcat(view, bufferFile);
	fclose(indexHtml);

	return view;
}

View weatherView(View view, char* address)
{
	if (address == NULL)
	{
		return strcat(view, "no address");
	}
	weatherInfo* weather = searchWeather(address);
	char buffer[24] = { 0 };
	free(address);

	strcat(view, u8"城市：");
	strcat(view, weather->city);
	strcat(view, "</br>");
	strcat(view, u8"天气：");
	strcat(view, weather->weather);
	strcat(view, "</br>");
	strcat(view, u8"风向：");
	strcat(view, weather->winddirection);
	strcat(view, "</br>");
	strcat(view, u8"风力等级：");
	strcat(view, itoa(weather->windpower, buffer, 10));
	strcat(view, "</br>");
	strcat(view, u8"空气湿度：");
	strcat(view, itoa(weather->humidity, buffer, 10));
	strcat(view, "</br>");
	strcat(view, u8"温度：");
	strcat(view, itoa(weather->temperature, buffer, 10));
	strcat(view, "</br>");
	strcat(view, u8"报道时间：");
	strcat(view, weather->reporttime);

	free(weather);
	return view;
}

View weather_predictView(View view, char *address)
{
	if (address == NULL)
	{
		return strcat(view, "no address");
	}
	weather_predictInfo* weather = predictWeather(address);
	free(address);
	
	strcat(view, u8"城市：");
	strcat(view, weather->city);
	strcat(view, "</br>");
	strcat(view, u8"报道时间：");
	strcat(view, "</br>");
	strcat(view, weather->reporttime);
	int i;
	for (i = 0; i <= 2; i++)
	{
		strcat(view, u8"未来天气</br>");
		strcat(view, u8"早上气温");
		strcat(view, weather->daytemp[i]);
		strcat(view, "</br>");
		
	}

	free(weather);
	return view;
}

View saveView(View view, char* request)
{
	char* post[4096] = { 0 };
	fetchContent(request, post);
	decode(post);
	strcat(post, SEPARATOR);
	
	
	FILE* database = fopen("database", "ab+");

	fwrite(post, 1, strlen(post), database);
	
	fclose(database);

	return strcat(view, u8"<h2>保存成功</h2>");
}

View showView(View view)
{
	linedList* pnode = create_linedList();
	if (pnode == NULL)
	{
		return strcat(view, u8"<h2>暂无信息</h2>");
	}
	while(pnode)
	{
		strcat(view, u8"地点：");
		strcat(view, pnode->title);
		strcat(view, "</br>");
		strcat(view, u8"记录：");
		strcat(view, pnode->content);
		strcat(view, "</br>");
		pnode = pnode->next;
	}
	free_linedList(pnode);
	return view;
}