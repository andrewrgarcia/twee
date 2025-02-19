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
#include <stdbool.h>

#define EXT_COLOR "\033[38;2;221;45;97m" 
#define MAX_LINES 2000

#ifdef ENABLE_PDF
#include <poppler/glib/poppler.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

void show_pdf_content(const char *filename) {
    GError *error = NULL;

    // Convert relative path to absolute URI
    gchar *absolute_path = realpath(filename, NULL);
    if (!absolute_path) {
        printf("❌ Error resolving file path: %s\n", filename);
        return;
    }

    gchar *uri = g_strdup_printf("file://%s", absolute_path);
    free(absolute_path);  // realpath() allocates memory, free it

    PopplerDocument *doc = poppler_document_new_from_file(uri, NULL, &error);
    g_free(uri);  // Free the dynamically allocated URI string

    if (!doc) {
        printf("❌ Error reading PDF: %s\n", error->message);
        g_error_free(error);
        return;
    }

    int num_pages = poppler_document_get_n_pages(doc);
    printf("📕 PDF Content (%s):\n", filename);
    
    for (int i = 0; i < num_pages; i++) {
        PopplerPage *page = poppler_document_get_page(doc, i);
        char *text = poppler_page_get_text(page);  // ✅ Correct API usage
        printf("%s\n", text);
        g_free(text);
        g_object_unref(page);
    }

    g_object_unref(doc);
}
#endif


void show_file_contents(char **files, int file_count, const Config *config) {
    printf("\n===============================================\n\n");
    for (int i = 0; i < file_count; i++) {
        const char *filename = files[i];
        const char *ext = strrchr(filename, '.');

        if (!ext) continue;  // Skip files without extensions

        // ✅ Check if file extension is allowed (including PDFs)
        bool should_show = (config->num_show_extensions == 0);  // Show all if no filter
        for (int j = 0; j < config->num_show_extensions; j++) {
            if (strcmp(ext + 1, config->show_extensions[j]) == 0) {
                should_show = true;
                break;
            }
        }
        if (!should_show) continue;  // Skip files not in `--show` filter

#ifdef ENABLE_PDF
        if (strcmp(ext, ".pdf") == 0) {
            show_pdf_content(filename);  // Call PDF reader
            continue;
        }
#endif

        printf("<<< FILE START: %s >>>\n", filename);
        FILE *file = fopen(filename, "r");
        if (!file) {
            printf("❌ Error opening file: %s\n", filename);
            continue;
        }

        char line[1024];
        int line_num = 1;
        int total_lines = 0;
        int skip_lines = 0;
        // Count total lines if tail is used
        if (config->tail_lines > 0) {
            while (fgets(line, sizeof(line), file)) total_lines++;
            rewind(file);
            skip_lines = total_lines - config->tail_lines;
            if (skip_lines < 0) skip_lines = 0;
        }

        // Read and print file content based on head/tail constraints
        while (fgets(line, sizeof(line), file)) {
            if (config->tail_lines > 0 && line_num <= skip_lines) {
                line_num++;
                continue;
            }
            if (config->head_lines > 0 && line_num > config->head_lines) break;
            if (line_num > MAX_LINES) {
                printf("[Truncated at %d lines...]\n", MAX_LINES);
                break;
            }
            printf("%s", line);
            line_num++;
        }
        fclose(file);

        printf("\n<<< FILE END: %s >>>\n\n", filename);
    }
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

void list_directory(const char *base_path, int depth, const Config *config, 
                    char **ignore_patterns, int ignore_count, char ***files, int *file_count) {
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
        const char *ext = strrchr(entry->d_name, '.');


        // Apply --show filter: Only list specified extensions
        bool should_show = (config->num_show_extensions == 0);  // Default to all if no filter is set
        if (!should_show) {
            for (int i = 0; i < config->num_show_extensions; i++) {
                if (ext && strcmp(ext + 1, config->show_extensions[i]) == 0) {
                    should_show = true;
                    break;
                }
            }
        }

        if (!should_show && !is_directory) continue;  // Skip files that don't match --show

        // Flat format (./path/to/file.c)
        if (!config->use_tree) {
            if (is_directory) {
                list_directory(path, depth + 1, config, ignore_patterns, ignore_count, files, file_count);
            } else {
                printf("%s%s\n", get_type_icon(entry->d_name, is_directory, config),path);
            }
            continue;
        }

        // Tree format
        for (int i = 0; i < depth; i++) printf("│   ");
        printf("├── ");

        if (config->use_index) {
            printf("%s\n", entry->d_name);
        } else {
            if (config->show_details) {
                time_t mod_time_raw = path_stat.st_mtime;
                struct tm *mod_time = localtime(&mod_time_raw);
                char time_str[20];
                strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", mod_time);
                printf("%s [Size: %lld bytes] [Modified: %s]\n", entry->d_name, (long long) path_stat.st_size, time_str);
            } else {
                printf("%s %s\n", get_type_icon(entry->d_name, is_directory, config), entry->d_name);
            }
        }

        // Store matching files for later content display
        if (!is_directory && config->show_contents) {
            *files = realloc(*files, (*file_count + 1) * sizeof(char *));
            (*files)[*file_count] = strdup(path);
            (*file_count)++;
        }

        if (is_directory) {
            list_directory(path, depth + 1, config, ignore_patterns, ignore_count, files, file_count);
        }
    }
    closedir(dir);
}
