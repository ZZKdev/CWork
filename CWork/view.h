#pragma once

#define View char*
#define SEPARATOR "\r\n"
#define Request char*

View indexView(View view, Request request);
View weatherView(View view, Request request);
View weather_predictView(View, Request request);
View saveView(View view, Request request);
View showView(View view, Request requst);
View deleteView(View view, Request request);
View searchView(View view, Request request);
View sortView(View view, Request);