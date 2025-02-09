#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "config.h"

void list_directory(const char *base_path, int depth, const Config *config, char **ignore_patterns, int ignore_count);
void compare_directories(const char *path1, const char *path2);

#endif
