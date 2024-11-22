
#ifndef STARTUTILITY_H
#define STARTUTILITY_H

#include "cJSON.h"

char *ReadFileContent(const char *filename);
char *RefineJsonString(const char *input_json);
void ParseAndStoreJson(cJSON *json, const char *file, float *temp, char *humidity);

#endif
