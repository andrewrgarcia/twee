#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_config(Config *config) {
    config->use_emoji = true;
    config->show_details = false;
    config->use_gitignore = true;
    config->use_tree = true;
    config->use_index = false;
    config->max_depth = -1;
    config->show_contents = false;
    config->head_lines = 0;
    config->tail_lines = 0;
    config->show_extensions = NULL;
    config->num_show_extensions = 0;
}

void parse_arguments(int argc, char *argv[], Config *config, char **directory, char ***ignore_patterns, int *ignore_count) {
    *ignore_patterns = malloc(10 * sizeof(char*));
    config->show_extensions = malloc(10 * sizeof(char*));

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-L") == 0 && i + 1 < argc) {
            config->max_depth = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--ignore") == 0) {
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                (*ignore_patterns)[(*ignore_count)++] = strdup(argv[++i]);
            }
        } else if (strcmp(argv[i], "--show") == 0) {
            config->show_contents = true;
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                config->show_extensions[config->num_show_extensions++] = strdup(argv[++i]);
            }
        } else if (strcmp(argv[i], "--head") == 0 && i + 1 < argc) {
            config->head_lines = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--tail") == 0 && i + 1 < argc) {
            config->tail_lines = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--no-emoji") == 0) {
            config->use_emoji = false;
        } else if (strcmp(argv[i], "--details") == 0) {
            config->show_details = true;
        } else if (strcmp(argv[i], "--no-git") == 0) {
            config->use_gitignore = false;
        } else if (strcmp(argv[i], "--flat") == 0) {  // Changed meaning of `--flat`
            config->use_tree = false;
        } else if (strcmp(argv[i], "--index") == 0) {
            config->use_index = true;
        } else {
            *directory = argv[i];
        }
    }
}
