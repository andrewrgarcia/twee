#include "config.h"
#include "filesystem.h"
#include "ignore_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    Config config;
    init_config(&config);
    
    char *directory = ".";
    char **ignore_patterns = NULL;
    int ignore_count = 0;
    
    if (argc == 4 && strcmp(argv[1], "--dif") == 0) {
        compare_structure(argv[2], argv[3], 0);
        return 0;
    }

    if (argc == 4 && strcmp(argv[1], "--diff") == 0) {
        compare_structure(argv[2], argv[3], 0);  // check structure
        printf("\n🔍 Now performing deep file comparison...\n");
        compare_files(argv[2], argv[3]);  // file contents
        return 0;
    }


    parse_arguments(argc, argv, &config, &directory, &ignore_patterns, &ignore_count);

    if (config.use_gitignore) {
        load_gitignore(&ignore_patterns, &ignore_count);
    }

    printf("📂 Listing directory: %s\n", directory);
    list_directory(directory, 0, &config, ignore_patterns, ignore_count);

    for (int i = 0; i < ignore_count; i++) {
        free(ignore_patterns[i]);
    }
    free(ignore_patterns);

    return 0;
}
