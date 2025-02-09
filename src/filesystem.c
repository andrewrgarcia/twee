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

void compare_files(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");

    if (!f1 || !f2) {
        perror("Error opening file");
        return;
    }

    char line1[1024], line2[1024];
    int line_num = 1;
    int differences_found = 0;
    
    while (1) {
        char *res1 = fgets(line1, sizeof(line1), f1);
        char *res2 = fgets(line2, sizeof(line2), f2);

        if (!res1 && !res2) break;  // Both files ended

        if (!res1) {
            printf("  + Line %d: %s", line_num, line2);  // Line exists only in file2
            differences_found++;
        } else if (!res2) {
            printf("  - Line %d: %s", line_num, line1);  // Line exists only in file1
            differences_found++;
        } else if (strcmp(line1, line2) != 0) {
            printf("  ⚠️ Difference at line %d:\n", line_num);
            printf("  - %s", line1);
            printf("  + %s", line2);
            differences_found++;
        }
        line_num++;
    }

    fclose(f1);
    fclose(f2);

    if (differences_found == 0) {
        printf("  ✅ No content differences found.\n");
    }
}

void compare_structure(const char *path1, const char *path2, int depth) {
    DIR *dir1 = opendir(path1);
    DIR *dir2 = opendir(path2);

    if (!dir1 || !dir2) {
        perror("Error opening directories");
        return;
    }

    struct dirent *entry;
    struct stat stat1, stat2;

    // 🔹 First, scan through directory 1
    while ((entry = readdir(dir1)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char file1[1024], file2[1024];
        snprintf(file1, sizeof(file1), "%s/%s", path1, entry->d_name);
        snprintf(file2, sizeof(file2), "%s/%s", path2, entry->d_name);

        if (stat(file1, &stat1) == 0) {
            if (stat(file2, &stat2) != 0) {
                for (int i = 0; i < depth; i++) printf("│   ");
                printf("📂 Only in %s: %s\n", path1, entry->d_name);
            } else if (S_ISREG(stat1.st_mode) && S_ISREG(stat2.st_mode)) {
                if (stat1.st_size != stat2.st_size) {
                    for (int i = 0; i < depth; i++) printf("│   ");
                    printf("⚠️ Differing files: %s (size mismatch)\n", entry->d_name);
                } 
                // 🔥 FIX: Now actually compare contents
                for (int i = 0; i < depth; i++) printf("│   ");
                printf("🔍 Comparing content of: %s\n", entry->d_name);
                compare_files(file1, file2);
            } 
            
            // 🔹 Ensure directories are compared recursively
            if (S_ISDIR(stat1.st_mode)) {
                for (int i = 0; i < depth; i++) printf("│   ");
                printf("📂 Entering directory: %s\n", entry->d_name);
                compare_structure(file1, file2, depth + 1);
            }
        }
    }
    closedir(dir1);

    // 🔹 Now scan directory 2 for files missing in directory 1
    while ((entry = readdir(dir2)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char file1[1024], file2[1024];
        snprintf(file1, sizeof(file1), "%s/%s", path1, entry->d_name);
        snprintf(file2, sizeof(file2), "%s/%s", path2, entry->d_name);

        if (stat(file1, &stat1) != 0) {  // File is missing in dir1
            for (int i = 0; i < depth; i++) printf("│   ");
            printf("📂 Only in %s: %s\n", path2, entry->d_name);
        }
    }
    closedir(dir2);
}

void list_directory(const char *base_path, int depth, const Config *config, char **ignore_patterns, int ignore_count) {
    if (config->max_depth >= 0 && depth > config->max_depth) return;

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
        
        // Print indentation for tree mode
        if (config->use_tree) {
            for (int i = 0; i < depth; i++) printf("│   ");
            printf("├── ");
        }

        printf("%s %s", get_type_icon(entry->d_name, is_directory, config), entry->d_name);

        if (config->show_details) {
            time_t mod_time_raw = path_stat.st_mtime;
            struct tm *mod_time = localtime(&mod_time_raw);
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", mod_time);
            printf(" [Size: %lld bytes] [Modified: %s]", (long long) path_stat.st_size, time_str);
        }
        printf("\n");

        if (is_directory) {
            list_directory(path, depth + 1, config, ignore_patterns, ignore_count);
        }
    }
    closedir(dir);
}
