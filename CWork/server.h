#pragma once

#include "cJSON.h"

cJSON* loadConfig();
int initServer();
void serverRun(int servSock);