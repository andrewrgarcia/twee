#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif

#define MAX_PATH_LENGTH 1024

bool use_emoji = true;
bool show_details = false;
bool use_gitignore = true;

bool ignore_match(const char *name, char **ignore_patterns, int ignore_count) {
    for (int i = 0; i < ignore_count; i++) {
        if (strstr(name, ignore_patterns[i]) != NULL) {
            return true;
        }
    }
    return false;
}

void load_gitignore(char *ignore_patterns[], int *ignore_count) {
    FILE *file = fopen(".gitignore", "r");
    if (!file) return;

    char line[MAX_PATH_LENGTH];
    while (fgets(line, sizeof(line), file) && *ignore_count < 10) {
        line[strcspn(line, "\n")] = 0;  // Remove newline character
        ignore_patterns[(*ignore_count)++] = strdup(line);
    }
    fclose(file);
}

const char* get_type_icon(const char *filename, bool is_directory) {
    if (!use_emoji) return "";
    if (is_directory) return "📁";
    if (strstr(filename, ".txt")) return "📄";
    if (strstr(filename, ".py")) return "🐍";
    if (strstr(filename, ".md")) return "📜";
    if (strstr(filename, ".zip") || strstr(filename, ".tar") || strstr(filename, ".gz")) return "📦";
    if (strstr(filename, ".jpg") || strstr(filename, ".png") || strstr(filename, ".svg")) return "🖼";
    if (strstr(filename, ".mp4") || strstr(filename, ".mov")) return "🎥";
    if (strstr(filename, ".mp3") || strstr(filename, ".wav")) return "🎵";
    return "📄"; // Default for files
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

        bool is_directory = S_ISDIR(path_stat.st_mode);
        
        for (int i = 0; i < depth; i++) printf("  ");
        printf("%s %s", get_type_icon(entry->d_name, is_directory), entry->d_name);

        if (show_details) {
            printf(" [Size: %lld bytes]", (long long) path_stat.st_size);
            struct tm *mod_time = localtime(&path_stat.st_mtime);
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", mod_time);
            printf(" [Modified: %s]", time_str);
        }
        printf("\n");

        if (is_directory) {
            list_directory(path, depth + 1, max_depth, ignore_patterns, ignore_count);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    int max_depth = -1;
    char *directory = ".";
    char *ignore_patterns[10];
    int ignore_count = 0;

    if (use_gitignore) {
        load_gitignore(ignore_patterns, &ignore_count);
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-L") == 0 && i + 1 < argc) {
            max_depth = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--ignore") == 0 && i + 1 < argc) {
            ignore_patterns[ignore_count++] = argv[++i];
        } else if (strcmp(argv[i], "--no-emoji") == 0) {
            use_emoji = false;
        } else if (strcmp(argv[i], "--details") == 0) {
            show_details = true;
        } else if (strcmp(argv[i], "--no-git") == 0) {
            use_gitignore = false;
        } else {
            directory = argv[i];
        }
    }

    printf("📂 Listing directory: %s\n", directory);
    list_directory(directory, 0, max_depth, ignore_patterns, ignore_count);
    return 0;
}
