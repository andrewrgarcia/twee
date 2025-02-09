#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef _WIN32
    #include <windows.h>
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif

#define MAX_PATH_LENGTH 1024

bool ignore_match(const char *name, char **ignore_patterns, int ignore_count) {
    for (int i = 0; i < ignore_count; i++) {
        if (strstr(name, ignore_patterns[i]) != NULL) {
            return true;
        }
    }
    return false;
}

void list_directory(const char *base_path, int depth, int max_depth, char **ignore_patterns, int ignore_count) {
    if (max_depth >= 0 && depth > max_depth) return;

    DIR *dir = opendir(base_path);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (ignore_match(entry->d_name, ignore_patterns, ignore_count)) {
            continue;
        }

        char path[MAX_PATH_LENGTH];
        snprintf(path, sizeof(path), "%s%s%s", base_path, PATH_SEPARATOR, entry->d_name);

        struct stat path_stat;
        stat(path, &path_stat);

        for (int i = 0; i < depth; i++) printf("  ");
        if (S_ISDIR(path_stat.st_mode)) {
            printf("📂 %s\n", entry->d_name);
            list_directory(path, depth + 1, max_depth, ignore_patterns, ignore_count);
        } else {
            printf("📄 %s\n", entry->d_name);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    int max_depth = -1;
    char *directory = ".";
    char *ignore_patterns[10];
    int ignore_count = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-L") == 0 && i + 1 < argc) {
            max_depth = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--ignore") == 0 && i + 1 < argc) {
            ignore_patterns[ignore_count++] = argv[++i];
        } else {
            directory = argv[i];
        }
    }

    printf("📂 Listing directory: %s\n", directory);
    list_directory(directory, 0, max_depth, ignore_patterns, ignore_count);
    return 0;
}
