#include "ignore_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_IGNORE_PATTERNS 50

// Function to trim leading and trailing spaces
void trim_whitespace(char *str) {
    char *end;

    // Trim leading spaces
    while (isspace((unsigned char)*str)) str++;

    // If string is empty after trimming, return
    if (*str == 0) return;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null-terminate the trimmed string
    *(end + 1) = '\0';
}

void load_gitignore(char ***ignore_patterns, int *ignore_count) {
    FILE *file = fopen(".gitignore", "r");
    if (!file) return;

    *ignore_patterns = malloc(MAX_IGNORE_PATTERNS * sizeof(char *));
    char line[256];

    while (fgets(line, sizeof(line), file) && *ignore_count < MAX_IGNORE_PATTERNS) {
        trim_whitespace(line);  // Remove leading and trailing spaces

        // Ignore empty lines and comment lines
        if (line[0] == '#' || strlen(line) == 0) continue;

        // Handle wildcard `*` pattern correctly
        if (strchr(line, '*')) {
            // Add wildcard handling here if needed (basic support for now)
        }

        (*ignore_patterns)[(*ignore_count)++] = strdup(line);
    }

    fclose(file);
}


bool ignore_match(const char *name, char **ignore_patterns, int ignore_count) {
    for (int i = 0; i < ignore_count; i++) {
        // Check if name matches an exact ignore pattern
        if (strcmp(name, ignore_patterns[i]) == 0) return true;

        // Handle patterns with `*` (wildcards)
        if (strchr(ignore_patterns[i], '*')) {
            if (strstr(name, ignore_patterns[i] + 1) != NULL) return true;
        }
    }
    return false;
}

