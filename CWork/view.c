#include <stdio.h>
#include <string.h>
#include "view.h"

View indexView(View view)
{
	FILE* indexHtml = NULL;
	
	if ((indexHtml = fopen("index.html", "rb")) == NULL)
	{
		fprintf(stderr, "open index.html error!\n");
		return "404";
	}
	char bufferFile[2048] = { 0 };
	fread(bufferFile, 1, 428, indexHtml);
	strcat(view, bufferFile);
	fclose(indexHtml);

	return view;
}