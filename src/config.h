#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

typedef struct {
    bool use_emoji;
    bool show_details;
    bool use_gitignore;
    bool use_tree;   
    bool use_index;  
    int max_depth;
    bool show_contents;
    int head_lines;
    int tail_lines;
    char **show_extensions;
    int num_show_extensions;
    bool show_pdfs;
    char **only_dirs;
    int only_count;
    bool include_root;
#ifdef ENABLE_PDF
    bool pdf_enabled;
#endif
} Config;

void init_config(Config *config);
void parse_arguments(int argc, char *argv[], Config *config, char **directory, char ***ignore_patterns, int *ignore_count);

#endif
