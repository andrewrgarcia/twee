#include "filesystem.h"
#include "display.h"
#include "ignore_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void list_directory(const char *base_path, int depth, const Config *config, char **ignore_patterns, int ignore_count) {
    if (config->max_depth >= 0 && depth > config->max_depth) return;

    printf("DEBUG: Entering directory: %s\n", base_path);  // ADD THIS LINE
    
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

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);

        struct stat path_stat;
        stat(path, &path_stat);

        bool is_directory = S_ISDIR(path_stat.st_mode);
        
        for (int i = 0; i < depth; i++) printf("  ");
        printf("%s %s", get_type_icon(entry->d_name, is_directory, config), entry->d_name);

        if (config->show_details) {
            printf(" [Size: %lld bytes]", (long long) path_stat.st_size);
            struct tm *mod_time = localtime(&path_stat.st_mtime);
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", mod_time);
            printf(" [Modified: %s]", time_str);
        }
        printf("\n");

        if (is_directory) {
            list_directory(path, depth + 1, config, ignore_patterns, ignore_count);
        }
    }
    closedir(dir);
}
