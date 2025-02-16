#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

typedef struct {
    bool use_emoji;
    bool show_details;
    bool use_gitignore;
    bool use_tree;   
    bool use_index;  // When true, format for index mode
    int max_depth;
    bool show_contents;
    int head_lines;
    int tail_lines;
    char **show_extensions;  // Extensions allowed for `--show`
    int num_show_extensions;
} Config;

void init_config(Config *config);
void parse_arguments(int argc, char *argv[], Config *config, char **directory, char ***ignore_patterns, int *ignore_count);

#endif
