#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "InitializationTool.h"
#include "FinalizationTool.h"

int main() {
    const char *shell_script_path = "bash Script.sh";

    // Execute the Shell Script
    ExecuteScript(shell_script_path);

    // Read JSON-like Data from a File
    char *raw_data = ReadFileContent("TemporaryFile.txt");
    if (raw_data == NULL) {
        return 1;
    }

    // Clean up the JSON string
    char *structured_data = RefineJsonString(raw_data);
    free(raw_data);
    if (structured_data == NULL) {
        return 1;
    }

    // Parse the JSON string
    cJSON *parsed_json = cJSON_Parse(structured_data);
    free(structured_data);
    if (parsed_json == NULL) {
        const char *error_details = cJSON_GetErrorPtr();
        if (error_details != NULL) {
            fprintf(stderr, "Error in parsing JSON: %s\n", error_details);
        }
        return 1;
    }

    // Extracted Variables
    float temperature_value = 0.0;
    char humidity_level[15];

    // Extract and Store the Data
    ParseAndStoreJson(parsed_json, "Data.json", &temperature_value, humidity_level);

    // Example use case of Extracted Values
    if (temperature_value > 30) {
        printf("Caution:\n");
        printf("High temperature detected.\n");
    }

    // Execute Another Shell Script
    const char *report_script_path = "bash Report.sh";
    ExecuteScript(report_script_path);

    // Free Up Memory
    cJSON_Delete(parsed_json);

    return 0;
}
