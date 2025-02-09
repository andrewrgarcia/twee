#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

typedef struct {
    bool use_emoji;
    bool show_details;
    bool use_gitignore;
    int max_depth;
} Config;

void init_config(Config *config);
void parse_arguments(int argc, char *argv[], Config *config, char **directory, char ***ignore_patterns, int *ignore_count);

#endif
