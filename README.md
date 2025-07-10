[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.15033378.svg)](https://doi.org/10.5281/zenodo.15033378)

# 🌳 Twee - A Tree Viewer for the AI Era  

Twee is a fast and feature-rich alternative to `tree`, with **.gitignore awareness**, **file content preview (`--show`)**, and **built-in directory & file comparison (`--diff`)**. Twee respects `.gitignore`, ensuring accurate directory views of what's important.

The `--show` functionality in Twee is based on the efficient **"show" algorithm** from [`showfileshere`](https://github.com/andrewrgarcia/showfileshere), developed under the [MIT LICENSE](https://github.com/andrewrgarcia/showfileshere/blob/main/LICENSE).

https://github.com/user-attachments/assets/b054aec1-46f6-4644-9627-dc1d1a385757


## AI Injection


> **“Wow. That was an epic tale you just pasted into my mouth like a mom bird regurgitating a worm.”**
> — *Your AI, after `twee --show > show.txt`*

---


Twee's **file content preview (`--show`)** is an *underrated superpower*; it allows **structured extraction of codebases, configuration files, and entire directory snapshots** for use in **AI models, ChatGPT, and LLM-powered workflows** 📥🤖

### Use Cases:
✅ Inject an entire codebase into an AI-powered assistant for **context-aware debugging & refactoring**  
✅ Extract structured content for **automated documentation or static analysis**  
✅ Feed large repositories into **fine-tuned models & embeddings** without copy-pasting  

#### Example:
```sh
twee --show c h py > project_dump.txt
```
_(Feed `project_dump.txt` into ChatGPT or any LLM for deep analysis.)_

---

## 🚀 Features

| Feature                                      | `twee` | `tree` | `eza --tree` |
|----------------------------------------------|--------|--------|--------------|
| **Directory Tree View**                      | ✅ Yes | ✅ Yes | ✅ Yes |
| **File Size & Metadata (`--details`)**       | ✅ Yes | ❌ No | ✅ Yes |
| **Gitignore Awareness (Always On)**          | ✅ Yes | ❌ No | ✅ Yes |
| **File Differences (`--diff`)**              | ✅ Yes | ❌ No | ✅ Yes |
| **File Content Preview (`--show`)**          | ✅ Yes [*(AI-ready code extraction)*](#ai-injection) | ❌ No | ❌ No |
| **PDF Text Extraction (`--show pdf`)**       | ✅ [*(Optional)*](#optional-pdf-support) | ❌ No | ❌ No |
| **Flat View (`--flat`)**                     | ✅ Yes | ❌ No | ❌ No |
| **Written in C for Speed**                   | ✅ Yes | ❌ No | ❌ No |
---

## 📚 Usage Guide

```sh
Usage: twee [options] [directory]

Options:
  -h, --help         Show this help message and exit
  -L <level>         Limit directory depth to <level>
  --no-emoji         Disable emojis in output
  --details          Show file details (size, modified date)
  --show [exts]      Show contents of files with given extensions (e.g., `py js md pdf`)
  --ignore <name>    Ignore file/directory by name (can be used multiple times)
  --only <dir>...    Limit view to only the specified directories
                       Use +root to include top-level files (e.g. README.md)
  --no-git           Do not auto-ignore files listed in .gitignore
  --flat             Disable tree view (list as flat structure)
  --head <N>         Show only first N lines per file (used with --show)
  --tail <N>         Show only last N lines per file (used with --show)
  --dif <dir1> <dir2>   Compare directory structures (existence-only)
  --diff <dir1> <dir2>  Compare directory structures AND file contents  
```


---




### 📝 Examples

```sh
# Show tree of current directory
twee

# Limit depth to 2 levels
twee -L 2

# Disable emoji icons
twee --no-emoji

# Ignore multiple directories
twee --ignore node_modules build

# Only show files inside 'src' and top-level root files
twee --show --only src +root

# Show C/JSON/Markdown files from specific subdir, plus top-level root files (+root)
twee --show md c json --only tests/dir1 +root --index

# Compare structures between two directories
twee --dif dir1 dir2

# Compare structures AND file contents
twee --diff dir1 dir2

# Show contents of all `.py` and `.rs` files
twee --show py rs

# Show contents of PDFs (if built with PDF support)
twee --show pdf

# Show contents but limit output to first 10 lines per file
twee --show py --head 10
```

---
## 📦 Installation  

### Linux & macOS
```sh
git clone https://github.com/YOUR_USERNAME/twee.git
cd twee
make
sudo mv build/twee /usr/local/bin/twee
```

### Windows (via MinGW or WSL)
```sh
git clone https://github.com/YOUR_USERNAME/twee.git
cd twee
make
```
_(Optionally move `twee.exe` to a location in your PATH.)_

---

## Optional PDF Support

https://github.com/user-attachments/assets/1e5a4f86-6e18-4077-a1c5-f01188d8883d

Twee supports **PDF text extraction** with `--show pdf`, but it is not built by default.  
To enable **PDF reading**, install `poppler-glib` and build with the `pdf` target:

### Install Required Libraries
#### Debian/Ubuntu
```sh
sudo apt install libpoppler-glib-dev
```
#### Arch Linux
```sh
sudo pacman -S poppler-glib
```
#### Fedora
```sh
sudo dnf install poppler-glib-devel
```
#### macOS (Homebrew)
```sh
brew install poppler
```

### Build with PDF Support
```sh
make pdf
```

Now, Twee can **extract text from PDFs** using:
```sh
twee --show pdf
```

---

## 🌍 Global Usage

To make `twee` available globally, add it to your shell configuration:

### Linux/macOS (Add to `.bashrc` or `.zshrc`)
```sh
echo 'twee() { "$HOME/twee/build/twee" "$@"; }' >> ~/.bashrc
source ~/.bashrc
```

### Windows (PowerShell)
```powershell
[System.Environment]::SetEnvironmentVariable("Path", $Env:Path + ";C:\\path\\to\\twee", [System.EnvironmentVariableTarget]::User)
```

---


## 🌟 **Why Twee?**
Twee isn't just a tree viewer—it’s a **developer’s productivity tool**:  

- 🎨 **Beautifully formatted output** with emoji-based file types  
- 🚀 **Fast execution**, even on large projects  
- 🛠️ **Built-in directory & file comparisons**, replacing `diff -rq`  
- 🔥 **Git-aware** it automatically respects `.gitignore`  
- 📂 **File Content Previewing (`--show`)** perfect for AI workflows, code analysis, and LLM-powered tools  
- 📖 **Optional PDF text extraction (`--show pdf`)**  
- ⚡ **Written in C for speed**, ensuring optimal performance  

🔗 **GitHub Repository:** [github.com/andrewrgarcia/twee](https://github.com/andrewrgarcia/twee)  

✨ Star the repo if you like it! 🚀

## 📄 Citation
If you use Twee in your research, please cite:

> **Garcia, A. R.** (2025). *Twee: Lightweight Git-aware File Extraction for AI-driven Code Analysis*. Zenodo. [https://doi.org/10.5281/zenodo.15033378](https://doi.org/10.5281/zenodo.15033378)

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.15033378.svg)](https://doi.org/10.5281/zenodo.15033378)
