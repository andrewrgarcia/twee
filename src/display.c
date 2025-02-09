#include "display.h"
#include <string.h>

const char* get_type_icon(const char *filename, bool is_directory, const Config *config) {
    if (!config->use_emoji) return "";
    if (is_directory) return "📁";
    if (strstr(filename, ".txt")) return "📄";
    if (strstr(filename, ".py")) return "🐍";
    if (strstr(filename, ".md")) return "📜";
    if (strstr(filename, ".zip") || strstr(filename, ".tar") || strstr(filename, ".gz")) return "📦";
    if (strstr(filename, ".jpg") || strstr(filename, ".png") || strstr(filename, ".svg")) return "🖼";
    if (strstr(filename, ".mp4") || strstr(filename, ".mov")) return "🎥";
    if (strstr(filename, ".mp3") || strstr(filename, ".wav")) return "🎵";
    if (strstr(filename, ".rs")) return "🦀";
    return "📄";
}
