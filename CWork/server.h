#pragma once

#define log(M, ...) printf("LOG AT %d: " M "\n", __LINE__, __VA_ARGS__)
#define check(EXPR) if (!EXPR) goto error;
#include "cJSON.h"

cJSON* loadConfig();
int initServer();
void serverRun();