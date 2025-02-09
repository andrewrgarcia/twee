#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "config.h"

void list_directory(const char *base_path, int depth, const Config *config, char **ignore_patterns, int ignore_count);

#endif
