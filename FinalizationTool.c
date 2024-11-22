
#include <stdio.h>
#include <stdlib.h>
#include "FinalizationTool.h"

// Shell Script ko execute krega
void ExecuteScript(const char *script_path) {
    int result = system(script_path);
    if (result != 0) {
        fprintf(stderr, "Error executing script: %s\n", script_path);
    }
}
