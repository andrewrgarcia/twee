#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "config.h"

void list_directory(const char *base_path, int depth, const Config *config, 
                    char **ignore_patterns, int ignore_count, char ***files, int *file_count);

void compare_structure(const char *path1, const char *path2, int depth, bool compare_contents);
void compare_files(const char *file1, const char *file2, int depth);
void show_file_contents(char **files, int file_count, const Config *config);

#endif
