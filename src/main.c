#include "config.h"
#include "filesystem.h"
#include "ignore_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help() {
    printf("📂 Twee - A modern tree viewer with file comparison 📂\n");
    printf("--------------------------------------------------------\n");
    printf("Usage:\n");
    printf("  twee [options] [directory]\n\n");

    printf("Options:\n");
    printf("  -h, --help                Show this help message and exit\n");
    printf("  -L <level>                Limit directory depth to <level>\n");
    printf("  --flat                    Show full file paths AND tree format\n");
    printf("  --no-emoji                Disable emojis in output\n");
    printf("  --details                 Show file details (size, modified date)\n");
    printf("  --no-git                  Do not auto-ignore files listed in .gitignore\n");
    printf("  --show <ext1> ...         Show only specific file extensions (e.g., 'c h')\n");
    printf("  --index                   Only show matching filenames (requires --show)\n");
    printf("  --ignore <name>...        Ignore file/directory by name (requires --show)\n");
    printf("  --only <dir>...           Limit view to specified directories (requires --show)\n");
    printf("                            Use '+root' to include top-level files\n");
    printf("  --dif <dir1> <dir2>       Compare directory structures (existence-only)\n");
    printf("  --diff <dir1> <dir2>      Compare directory structures AND file contents\n\n");

    printf("Examples:\n");
    printf("  twee                          # Show tree of current directory\n");
    printf("  twee -L 2                     # Limit depth to 2 levels\n");
    printf("  twee --flat                   # List files as './src/main.c' etc.\n");
    printf("  twee --show c h               # Show only C and header files\n");
    printf("  twee --show py --index        # Show index view of Python files\n");
    printf("  twee --show --index           # Only list file names\n");
    printf("  twee --show --only src +root  # Only show src/ and root-level files\n");
    printf("  twee --no-emoji                   # Disable emoji icons\n");
    printf("  twee --ignore node_modules    # Ignore specific directories\n");
    printf("  twee --dif dir1 dir2          # Show file existence differences\n");
    printf("  twee --diff dir1 dir2         # Show full content + structure diff\n");

    printf("--------------------------------------------------------\n");
    printf("🌟 Created by YOU | Open-source alternative to 'tree' and 'exa --tree'\n");
    printf("🔗 GitHub: \033[36mhttps://github.com/andrewrgarcia/twee\033[0m\n\n");
}




int main(int argc, char *argv[]) {
    Config config;
    init_config(&config);
    
    char *directory = ".";
    char **ignore_patterns = NULL;
    int ignore_count = 0;
    
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        print_help();
        return 0;
    }

    if (argc == 4 && strcmp(argv[1], "--dif") == 0) {
        compare_structure(argv[2], argv[3], 0, false);  // Structure-only comparison
        return 0;
    }

    if (argc == 4 && strcmp(argv[1], "--diff") == 0) {
        compare_structure(argv[2], argv[3], 0, true);  // Full comparison including file contents
        return 0;
    }

    parse_arguments(argc, argv, &config, &directory, &ignore_patterns, &ignore_count);

    if (config.use_gitignore) {
        load_gitignore(&ignore_patterns, &ignore_count);
    }

    printf("📂 Listing directory: %s\n", directory);

    // ✅ Store file paths for later content display
    char **files = NULL;
    int file_count = 0;

    // ✅ First, print **flat structure** if `--flat` or `--show` is enabled
    if (!config.use_tree || config.show_contents) {
        config.use_tree = false;  // Force flat mode
        list_directory(directory, 0, &config, ignore_patterns, ignore_count, &files, &file_count);
        config.use_tree = true;   // Reset for tree mode
        file_count = 0;  // Reset file count before tree listing
        printf("\n");  // Spacing before tree view
    }

    // ✅ Then, print **tree view**
    if (!config.use_index) {
        list_directory(directory, 0, &config, ignore_patterns, ignore_count, &files, &file_count);
    }

    // ✅ Finally, print file contents if `--show` is enabled
    if (config.show_contents) {
        show_file_contents(files, file_count, &config);
    }

    // ✅ Free dynamically allocated memory
    for (int i = 0; i < ignore_count; i++) {
        free(ignore_patterns[i]);
    }
    free(ignore_patterns);

    for (int i = 0; i < file_count; i++) {
        free(files[i]);
    }
    free(files);

    for (int i = 0; i < config.num_show_extensions; i++) {
        free(config.show_extensions[i]);
    }
    free(config.show_extensions);

    for (int i = 0; i < config.only_count; i++) {
        free(config.only_dirs[i]);
    }
    free(config.only_dirs);


    return 0;
}
