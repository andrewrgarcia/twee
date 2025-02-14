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

#define EXT_COLOR "\033[38;2;221;45;97m" 

void show_file_contents(const char *filename, int depth, const Config *config) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        for (int i = 0; i < depth; i++) printf("│   ");
        printf("❌ Error opening file: %s\n", filename);
        return;
    }

    printf("<<< FILE START: %s >>>\n", filename);

    char line[1024];
    int line_num = 0;
    int total_lines = 0;
    int skip_lines = 0;

    // Count total lines for tail processing
    if (config->tail_lines > 0) {
        while (fgets(line, sizeof(line), file)) total_lines++;
        rewind(file);
        skip_lines = total_lines - config->tail_lines;
        if (skip_lines < 0) skip_lines = 0;
    }

    // Read & print file content
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        if (config->head_lines > 0 && line_num > config->head_lines) break;
        if (config->tail_lines > 0 && line_num <= skip_lines) continue;

        for (int i = 0; i < depth + 1; i++) printf("│   ");
        printf("%s", line);
    }

    printf("<<< FILE END: %s >>>\n\n", filename);

    fclose(file);
}


void compare_files(const char *file1, const char *file2, int depth) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");

    if (!f1 || !f2) {
        perror("Error opening file");
        return;
    }

    char line1[1024], line2[1024];
    int line_num = 1;
    int differences_found = 0;

    printf("\n");  // 🔹 Ensure separation before file comparison output

    while (1) {
        char *res1 = fgets(line1, sizeof(line1), f1);
        char *res2 = fgets(line2, sizeof(line2), f2);

        if (!res1 && !res2) break;  // Both files ended

        // 🔹 Print indentation aligned with tree
        for (int i = 0; i < depth + 1; i++) printf("│   ");

        if (!res1) {
            printf("\033[32m+ Line %d: %s\033[0m", line_num, line2);  // Green (File 2 extra lines)
            differences_found++;
        } else if (!res2) {
            printf("\033[31m- Line %d: %s\033[0m", line_num, line1);  // Red (File 1 extra lines)
            differences_found++;
        } else if (strcmp(line1, line2) != 0) {
            printf("⚠️ Difference at line %d:\n", line_num);

            for (int i = 0; i < depth + 2; i++) printf("│   ");  // Correctly indent line diffs
            printf("\033[31m- %s\033[0m", line1);  // Red (removed line)

            for (int i = 0; i < depth + 2; i++) printf("│   ");
            printf("\033[32m+ %s\033[0m\n", line2);  // Green (added line)

            differences_found++;
        }
        line_num++;
    }

    fclose(f1);
    fclose(f2);

    if (differences_found == 0) {
        for (int i = 0; i < depth + 1; i++) printf("│   ");
        printf("✅ No content differences found.\n");
    }

    printf("\n");  // 🔹 Ensure spacing before the next directory entry
}

void compare_structure(const char *path1, const char *path2, int depth, bool compare_contents) {
    DIR *dir1 = opendir(path1);
    DIR *dir2 = opendir(path2);

    if (!dir1 || !dir2) {
        perror("Error opening directories");
        return;
    }

    struct dirent *entry;
    struct stat stat1, stat2;

    // 🔹 Scan through directory 1
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
                // Only compare contents when `compare_contents == true`
                if (compare_contents) {
                    for (int i = 0; i < depth; i++) printf("│   ");
                    printf("🔍 Comparing content of: %s\n", entry->d_name);
                    
                    compare_files(file1, file2, depth); 
                }


            } 
            
            // 🔹 Ensure directories are compared recursively
            if (S_ISDIR(stat1.st_mode)) {
                for (int i = 0; i < depth; i++) printf("│   ");
                printf("📂 Entering directory: %s\n", entry->d_name);
                compare_structure(file1, file2, depth + 1, compare_contents);
            }
        }
    }
    closedir(dir1);

    // 🔹 Scan directory 2 for files missing in directory 1
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

        // ✅ File extension coloring
        const char *reset = "\033[0m";
        const char *ext = strrchr(entry->d_name, '.');

        if (ext) {
            if (!config->show_contents) {  // Apply color only when NOT using --show
                printf("%s %.*s%s%s%s", 
                    get_type_icon(entry->d_name, is_directory, config), 
                    (int)(ext - entry->d_name), entry->d_name,  // Filename without extension
                    EXT_COLOR, ext, reset  // Apply color only to extension
                );
            } else {
                printf("%s %s", get_type_icon(entry->d_name, is_directory, config), entry->d_name);
            }
        } else {
            printf("%s %s", get_type_icon(entry->d_name, is_directory, config), entry->d_name);
        }


        // ✅ File details
        if (config->show_details) {
            time_t mod_time_raw = path_stat.st_mtime;
            struct tm *mod_time = localtime(&mod_time_raw);
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", mod_time);
            printf(" [Size: %lld bytes] [Modified: %s]", (long long) path_stat.st_size, time_str);
        }

        printf("\n");

        // ✅ If it's a directory, recurse
        if (is_directory) {
            list_directory(path, depth + 1, config, ignore_patterns, ignore_count);
        }

        // ✅ Handle file content previewing (`--show`)
        if (!is_directory && config->show_contents) {
            // Check if the file extension matches a specified one
            bool should_show = false;
            if (config->num_show_extensions == 0) {
                should_show = true; // No filter means show all files
            } else {
                for (int i = 0; i < config->num_show_extensions; i++) {
                    if (ext && strcmp(ext + 1, config->show_extensions[i]) == 0) { // +1 to skip dot
                        should_show = true;
                        break;
                    }
                }
            }

            if (should_show) {
                // ✅ Read file contents with head/tail constraints
                FILE *fp = fopen(path, "r");
                if (fp) {
                    printf("│   │   📄 Contents of %s:\n", entry->d_name);
                    char line[1024];
                    int line_num = 0;
                    int total_lines = 0;
                    int skip_lines = 0;

                    // First, count total lines if tail is used
                    if (config->tail_lines > 0) {
                        while (fgets(line, sizeof(line), fp)) total_lines++;
                        rewind(fp);
                        skip_lines = total_lines - config->tail_lines;
                        if (skip_lines < 0) skip_lines = 0;
                    }

                    // Read file again for output
                    while (fgets(line, sizeof(line), fp)) {
                        line_num++;
                        if (config->head_lines > 0 && line_num > config->head_lines) break;
                        if (config->tail_lines > 0 && line_num <= skip_lines) continue;

                        for (int i = 0; i < depth + 2; i++) printf("│   ");
                        printf(" %s", line);
                    }

                    fclose(fp);
                } else {
                    for (int i = 0; i < depth + 2; i++) printf("│   ");
                    printf("❌ Cannot open file: %s\n", entry->d_name);
                }
            }
        }
    }
    closedir(dir);
}
