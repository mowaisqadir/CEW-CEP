#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "InitializationTool.h"

char *ReadFileContent(const char *filename) {
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL) {
        perror("File opening failed");
        return NULL;
    }

    fseek(input_file, 0, SEEK_END);
    long size = ftell(input_file);
    rewind(input_file);

    char *data = (char *)malloc(size + 1);
    if (data == NULL) {
        perror("Failed memory allocation");
        fclose(input_file);
        return NULL;
    }

    fread(data, 1, size, input_file);
    data[size] = '\0';
    fclose(input_file);

    return data;
}

char *RefineJsonString(const char *input_json) {
    size_t length = strlen(input_json);
    char *output_json = (char *)malloc(length + 1);
    if (output_json == NULL) {
        perror("Memory allocation error");
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < length; i++) {
        if (input_json[i] == '\\' && (input_json[i + 1] == '"' || input_json[i + 1] == '\\')) {
            continue;
        }
        output_json[j++] = input_json[i];
    }
    output_json[j] = '\0';

    return output_json;
}

void ParseAndStoreJson(cJSON *json, const char *file, float *temp, char *humidity) {
    cJSON *new_entry = cJSON_CreateObject();

    // Add current date and time
    {
        char current_time[25];
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        snprintf(current_time, sizeof(current_time), "%04d-%02d-%02d %02d:%02d:%02d",
                 t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        cJSON_AddStringToObject(new_entry, "timestamp", current_time);
    }

    cJSON *location_info = cJSON_GetObjectItem(json, "location");
    if (location_info) {
        cJSON *location_object = cJSON_CreateObject();
        cJSON_AddItemToObject(new_entry, "location_details", location_object);

        cJSON_AddStringToObject(location_object, "city", cJSON_GetStringValue(cJSON_GetObjectItem(location_info, "name")));
        cJSON_AddStringToObject(location_object, "area", cJSON_GetStringValue(cJSON_GetObjectItem(location_info, "region")));
        cJSON_AddStringToObject(location_object, "nation", cJSON_GetStringValue(cJSON_GetObjectItem(location_info, "country")));
    }

    cJSON *current_conditions = cJSON_GetObjectItem(json, "current");
    if (current_conditions) {
        cJSON *conditions_object = cJSON_CreateObject();
        cJSON_AddItemToObject(new_entry, "weather_conditions", conditions_object);

        cJSON_AddNumberToObject(conditions_object, "temperature", cJSON_GetObjectItem(current_conditions, "temp_c")->valuedouble);
        cJSON_AddNumberToObject(conditions_object, "humidity", cJSON_GetObjectItem(current_conditions, "humidity")->valueint);

        *temp = cJSON_GetObjectItem(current_conditions, "temp_c")->valuedouble;
        snprintf(humidity, 15, "%d%%", cJSON_GetObjectItem(current_conditions, "humidity")->valueint);
    }

    cJSON *array = NULL;
    FILE *existing_file = fopen(file, "r");
    if (existing_file) {
        fseek(existing_file, 0, SEEK_END);
        long file_size = ftell(existing_file);
        rewind(existing_file);

        char *content = (char *)malloc(file_size + 1);
        if (content) {
            fread(content, 1, file_size, existing_file);
            content[file_size] = '\0';
            array = cJSON_Parse(content);
            free(content);
        }
        fclose(existing_file);
    }

    if (!array || !cJSON_IsArray(array)) {
        if (array) {
            cJSON_Delete(array);
        }
        array = cJSON_CreateArray();
    }

    cJSON_AddItemToArray(array, new_entry);

    char *output_string = cJSON_Print(array);
    FILE *output_file = fopen(file, "w");
    if (output_file) {
        fprintf(output_file, "%s\n", output_string);
        fclose(output_file);
    }

    cJSON_Delete(array);
    free(output_string);
}
