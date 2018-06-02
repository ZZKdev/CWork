#pragma once

#define View char*

View indexView(View view);
View weatherView(View view, char* address);
View weather_predictView(View, char* address);