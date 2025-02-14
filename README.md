# 🌳 Twee - A Modern Tree Viewer with File Comparison

Twee is a fast and feature-rich alternative to `tree`, with built-in **file comparison**, **.gitignore awareness**, **file content previewing (`--show`)**, and **emoji-based visualization**. Unlike `exa --tree`, Twee **always** respects `.gitignore`, ensuring accurate directory views.

## 🚀 Features

| Feature                                     | `twee` | `tree` | `exa --tree` |
|---------------------------------------------|--------|--------|--------------|
| **Directory Tree View**                     | ✅ Yes | ✅ Yes | ✅ Yes |
| **File Size & Metadata**                    | ✅ Yes (`--details`) | ❌ No | ✅ Yes |
| **Gitignore Awareness**                     | ✅ Always | ❌ No | ⚠️ Maybe (depends on build) |
| **File Differences (`--diff`)**             | ✅ Yes | ❌ No | ✅ Yes |
| **File Content Preview (`--show`)**         | ✅ Yes | ❌ No | ❌ No |
| **Emoji Representation**                    | ✅ Yes | ❌ No | ❌ No |
| **Flat View (`--flat`)**                    | ✅ Yes | ❌ No | ❌ No |
| **Written in C for Speed**                  | ✅ Yes | ❌ No | ❌ No |

---

## 📚 Installation

### Linux & macOS

```sh
# Clone repository
git clone https://github.com/YOUR_USERNAME/twee.git
cd twee

# Build executable
make

# Install globally
sudo mv build/twee /usr/local/bin/twee
```

### Windows (via MinGW or WSL)

```sh
git clone https://github.com/YOUR_USERNAME/twee.git
cd twee
make
# Optionally move twee.exe to a location in your PATH
```

---

## 🌍 Global Usage

To make `twee` available globally, add it to your shell configuration:

**Linux/macOS** (Add to `.bashrc` or `.zshrc`)

```sh
echo 'twee() { "$HOME/twee/build/twee" "$@"; }' >> ~/.bashrc
source ~/.bashrc
```

**Windows (PowerShell)**

```powershell
[System.Environment]::SetEnvironmentVariable("Path", $Env:Path + ";C:\\path\\to\\twee", [System.EnvironmentVariableTarget]::User)
```

---

## 📚 Usage Guide

```sh
Usage: twee [options] [directory]

Options:
  -h, --help         Show this help message and exit
  -L <level>         Limit directory depth to <level>
  --no-emoji         Disable emojis in output
  --details          Show file details (size, modified date)
  --ignore <name>    Ignore file/directory by name (can be used multiple times)
  --no-git           Do not auto-ignore files listed in .gitignore
  --flat             Disable tree view (list as flat structure)
  --show [exts]      Show contents of files with given extensions
  --head <N>         Show only first N lines per file (used with --show)
  --tail <N>         Show only last N lines per file (used with --show)
  --dif <dir1> <dir2>   Compare directory structures (existence-only)
  --diff <dir1> <dir2>  Compare directory structures AND file contents
```

### Examples:

```sh
# Show tree of current directory
twee

# Limit depth to 2 levels
twee -L 2

# Disable emoji icons
twee --no-emoji

# Ignore multiple directories
twee --ignore node_modules --ignore build

# Compare structures between two directories
twee --dif dir1 dir2

# Compare structures AND file contents
twee --diff dir1 dir2

# Show contents of all `.py` and `.rs` files
twee --show py rs

# Show contents but limit output to first 10 lines per file
twee --show py --head 10
```

---

## 🌟 Why Twee?

Twee offers a **better user experience** with:

- 🎨 **Beautiful output** with emoji-based file types
- 🚀 **Fast execution**, even on large projects
- 🛠️ **Built-in diffing capabilities**, replacing `diff -rq`
- 🔥 **Always respects .gitignore**, unlike `exa --tree`
- 💡 **File Content Previewing**, useful for quick code inspection and development
- ⚡ **Written in C for speed**, ensuring optimal performance

🔗 **GitHub Repository:** [github.com/andrewrgarcia/twee](https://github.com/andrewrgarcia/twee)

✨ Star the repo if you like it! 🚀

