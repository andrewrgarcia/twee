#include "display.h"
#include <string.h>

const char* get_type_icon(const char *filename, bool is_directory, const Config *config) {
    if (!config->use_emoji) return "";

    if (is_directory) return "📁";

    // Extract base filename (ignore path)
    const char *basename = strrchr(filename, '/');
    if (!basename) {
        basename = filename; // No '/' found, filename is already the base name
    } else {
        basename++; // Move past the '/'
    }

    // Handle special filenames **without** extensions
    if (strcmp(basename, "Makefile") == 0) return "🔨";
    if (strcmp(basename, "Dockerfile") == 0) return "🐋";

    // Extract file extension
    const char *ext = strrchr(basename, '.');
    if (!ext || ext == basename) return "📄"; // No extension or hidden file

    // Match exact extensions
    if (strcmp(ext, ".txt") == 0) return "📄";
    if (strcmp(ext, ".pdf") == 0) return "📕";
    if (strcmp(ext, ".py") == 0) return "🐍";
    if (strcmp(ext, ".rs") == 0) return "🦀";
    if (strcmp(ext, ".c") == 0) return "🅲";
    if (strcmp(ext, ".cpp") == 0) return "🔷";
    if (strcmp(ext, ".h") == 0 || strcmp(ext, ".hpp") == 0) return "📜";
    if (strcmp(ext, ".carbon") == 0) return "⚛️";
    if (strcmp(ext, ".js") == 0 || strcmp(ext, ".jsx") == 0) return "🟨";
    if (strcmp(ext, ".ts") == 0 || strcmp(ext, ".tsx") == 0) return "🟦";
    if (strcmp(ext, ".html") == 0) return "🟧";
    if (strcmp(ext, ".rb") == 0) return "💎"; // Ruby
    if (strcmp(ext, ".cr") == 0) return "💠"; // Crystal
    if (strcmp(ext, ".jl") == 0) return "💅"; // Julia
    if (strcmp(ext, ".md") == 0) return "📜";
    if (strcmp(ext, ".zip") == 0 || strcmp(ext, ".tar") == 0 || strcmp(ext, ".gz") == 0) return "📦";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0 || strcmp(ext, ".png") == 0 ||
        strcmp(ext, ".svg") == 0 || strcmp(ext, ".gif") == 0 || strcmp(ext, ".bmp") == 0 ||
        strcmp(ext, ".webp") == 0 || strcmp(ext, ".tiff") == 0 || strcmp(ext, ".heic") == 0 ||
        strcmp(ext, ".avif") == 0) return "🖼️ ";
    if (strcmp(ext, ".ico") == 0) return "🐱";
    if (strcmp(ext, ".mp4") == 0 || strcmp(ext, ".mov") == 0) return "🎥";
    if (strcmp(ext, ".mp3") == 0 || strcmp(ext, ".wav") == 0) return "🎵";

    return "📄";
}
