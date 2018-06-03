#pragma once

#define View char*
#define SEPARATOR "\r\n"

View indexView(View view);
View weatherView(View view, char* address);
View weather_predictView(View, char* address);
View saveView(View, char* request);