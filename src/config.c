#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_config(Config *config) {
    config->use_emoji = true;
    config->show_details = false;
    config->use_gitignore = true;
    config->max_depth = -1;
}

void parse_arguments(int argc, char *argv[], Config *config, char **directory, char ***ignore_patterns, int *ignore_count) {
    *ignore_patterns = malloc(10 * sizeof(char*));  // Allocate space for ignore patterns

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-L") == 0 && i + 1 < argc) {
            config->max_depth = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--ignore") == 0 && i + 1 < argc) {
            (*ignore_patterns)[(*ignore_count)++] = strdup(argv[++i]);
        } else if (strcmp(argv[i], "--no-emoji") == 0) {
            config->use_emoji = false;
        } else if (strcmp(argv[i], "--details") == 0) {
            config->show_details = true;
        } else if (strcmp(argv[i], "--no-git") == 0) {
            config->use_gitignore = false;
        } else {
            *directory = argv[i];
        }
    }
}
