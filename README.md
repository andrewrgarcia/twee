# 🌳 Twee - A Tree Viewer for the AI Era  

Twee is a fast and feature-rich alternative to `tree`, with **.gitignore awareness**, **file content preview (`--show`)**, and **built-in directory & file comparison (`--diff`)**. Twee respects `.gitignore`, ensuring accurate directory views of what's important.

The `--show` functionality in Twee is based on the efficient **"show" algorithm** from [`showfileshere`](https://github.com/andrewrgarcia/showfileshere), developed under the [MIT LICENSE](https://github.com/andrewrgarcia/showfileshere/blob/main/LICENSE)  

https://github.com/user-attachments/assets/0720b9be-9588-472b-9391-9ba9768d7dbd

## 🚀 Features  

| Feature                                      | `twee` | `tree` | `eza --tree` |
|----------------------------------------------|--------|--------|--------------|
| **Directory Tree View**                      | ✅ Yes | ✅ Yes | ✅ Yes |
| **File Size & Metadata (`--details`)**       | ✅ Yes | ❌ No | ✅ Yes |
| **Gitignore Awareness (Always On)**          | ✅ Yes | ❌ No | ✅ Yes |
| **File Differences (`--diff`)**              | ✅ Yes | ❌ No | ✅ Yes |
| **File Content Preview (`--show`)**          | ✅ Yes [*(AI-ready code extraction)*](#ai-injection) | ❌ No | ❌ No |
| **Flat View (`--flat`)**                     | ✅ Yes | ❌ No | ❌ No |
| **Written in C for Speed**                   | ✅ Yes | ❌ No | ❌ No |


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

### Examples

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

## AI Injection 📥🤖

Twee's **file content preview (`--show`)** is an *underrated superpower*—it allows **structured extraction of codebases, configuration files, and entire directory snapshots** for use in **AI models, ChatGPT, and LLM-powered workflows**.  

### Use Cases: 
✅ Inject an entire codebase into an AI-powered assistant for **context-aware debugging & refactoring**  
✅ Extract structured content for **automated documentation or static analysis**  
✅ Feed large repositories into **fine-tuned models & embeddings** without copy-pasting  

### Examples:

#### 🔹 **Capture all C and Python files for AI review**
```sh
twee --show c h py > project_dump.txt
```
_(Feed `project_dump.txt` into ChatGPT or any LLM for deep analysis.)_

#### 🔹 **Extract only the first 20 lines per file (for summarization)**
```sh
twee --show c h --head 20 > project_snippet.txt
```

#### 🔹 **Stream project code to an AI-powered tool (LLM integration)**
```sh
twee --show py js | AI_TOOL
```
_(Use this for **AI-assisted code refactoring, debugging, and documentation generation**.)_

---

## 🌟 Why Twee?  

Twee isn't just a tree viewer—it’s a **developer’s productivity tool**:  

- 🎨 **Beautifully formatted output** with emoji-based file types  
- 🚀 **Fast execution**, even on large projects  
- 🛠️ **Built-in directory & file comparisons**, replacing `diff -rq`  
- 🔥 **Git-aware**—it automatically respects `.gitignore`  
- 📂 **File Content Previewing (`--show`)**—perfect for AI workflows , code analysis, and LLM-powered tools  
- ⚡ **Written in C for speed**, ensuring optimal performance  

🔗 **GitHub Repository:** [github.com/andrewrgarcia/twee](https://github.com/andrewrgarcia/twee)

✨ Star the repo if you like it! 🚀

