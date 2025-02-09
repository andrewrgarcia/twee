#include "config.h"
#include "filesystem.h"
#include "ignore_handler.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    Config config;
    init_config(&config);
    
    char *directory = ".";
    char **ignore_patterns = NULL;
    int ignore_count = 0;

    parse_arguments(argc, argv, &config, &directory, &ignore_patterns, &ignore_count);

    if (config.use_gitignore) {
        load_gitignore(&ignore_patterns, &ignore_count);
    }

    printf("📂 Listing directory: %s\n", directory);
    list_directory(directory, 0, &config, ignore_patterns, ignore_count);
    
    // Free dynamically allocated ignore patterns
    for (int i = 0; i < ignore_count; i++) {
        free(ignore_patterns[i]);
    }
    free(ignore_patterns);

    return 0;
}
