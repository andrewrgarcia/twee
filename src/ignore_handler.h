#ifndef IGNORE_HANDLER_H
#define IGNORE_HANDLER_H

#include <stdbool.h>

void load_gitignore(char **ignore_patterns[], int *ignore_count);
bool ignore_match(const char *name, char **ignore_patterns, int ignore_count);

#endif
