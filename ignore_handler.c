#include "ignore_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_gitignore(char **ignore_patterns[], int *ignore_count) {
    FILE *file = fopen(".gitignore", "r");
    if (!file) return;

    char line[256];
    while (fgets(line, sizeof(line), file) && *ignore_count < 20) {
        line[strcspn(line, "\n")] = 0;
        (*ignore_patterns)[(*ignore_count)++] = strdup(line);
    }
    fclose(file);
}

bool ignore_match(const char *name, char **ignore_patterns, int ignore_count) {
    for (int i = 0; i < ignore_count; i++) {
        if (strstr(name, ignore_patterns[i]) != NULL) {
            return true;
        }
    }
    return false;
}
