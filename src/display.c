#include "display.h"
#include <string.h>

const char* get_type_icon(const char *filename, bool is_directory, const Config *config) {
    if (!config->use_emoji) return "";

    if (is_directory) return "📁";
    if (strstr(filename, ".txt")) return "📄";
    if (strstr(filename, ".py")) return "🐍";
    if (strstr(filename, ".rs")) return "🦀";
    if (strstr(filename, ".carbon")) return "⚛️";
    if (strstr(filename, ".js") || strstr(filename, ".jsx")) return "🟨";
    if (strstr(filename, ".ts") || strstr(filename, ".tsx")) return "🟦";
    if (strstr(filename, ".html")) return "🟧";
    if (strstr(filename, ".rb")) return "💎"; // Ruby
    if (strstr(filename, ".cr")) return "💠"; // Crystal
    if (strstr(filename, ".jl")) return "💅"; // Julia
    if (strstr(filename, ".md")) return "📜";
    if (strstr(filename, ".zip") || strstr(filename, ".tar") || strstr(filename, ".gz")) return "📦";
    if (strstr(filename, ".jpg") || strstr(filename, ".png") || strstr(filename, ".svg")) return "🖼";
    if (strstr(filename, ".mp4") || strstr(filename, ".mov")) return "🎥";
    if (strstr(filename, ".mp3") || strstr(filename, ".wav")) return "🎵";
    return "📄";
}
