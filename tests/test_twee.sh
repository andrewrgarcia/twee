#!/bin/bash

# Set execution permission: chmod +x test_twee.sh
# Run tests: ./test_twee.sh

LOG_FILE="twee_test.log"
echo "Running tests for twee..." > "$LOG_FILE"

run_test() {
    echo -e "\nRunning: $1" | tee -a "$LOG_FILE"
    eval "$1" | tee -a "$LOG_FILE"
}

# Basic directory listing
run_test "./build/twee"

# Limit depth to 0th level
run_test "./build/twee -L 0"

# Flat file listing
run_test "./build/twee --flat"

# Show only C and header files
run_test "./build/twee --show c h"

# Show Python files and index format
run_test "./build/twee --show py --index"

# List directory index only  # Expected to get nothing since --index only works with --show
run_test "./build/twee --index"

# Disable emojis
run_test "./build/twee --no-emoji"

# Disable .gitignore awareness & ignore .git/ dir
run_test "./build/twee --no-git --ignore .git"

# Ignore specific directories (incl .gitignore content)
run_test "./build/twee --ignore tests build"

# Compare directory structures
run_test "./build/twee --dif tests/dir1 tests/dir2"
run_test "./build/twee --diff tests/dir1 tests/dir2"


# Show only .txt files in tests/dir2 using --only
run_test "./build/twee --show txt --only tests/dir2"

# Show .c files only in nested subdir (src/utils under dir1)
run_test "./build/twee --show c --only tests/dir1/src"

# Flat listing for all header files under dir2
run_test "./build/twee --flat --show h --only tests/dir2"

# Check filtering at the file level: --only with a file path
run_test "./build/twee --show txt --only tests/dir2/nice.txt"

# Index view for just Python files under dir1
run_test "./build/twee --show py --only tests/dir1 --index"

# Tail behavior with head/tail configuration (simulate long file)
run_test "./build/twee --show txt --tail 5 --only tests/dir2"

# Include root-level files with --show (like sample.pdf, sample.tex)
run_test "./build/twee --show pdf tex --only tests"

# Attempt to load a nonexistent path
run_test "./build/twee --only tests/dir2/ghost --show rs"

# Compare nested utils folder
run_test "./build/twee --diff tests/dir1/src/utils tests/dir2/src/utils"

# Max depth 1 from root, verify partial crawl (should list only immediate children)
run_test "./build/twee -L 1 --show json --only tests"

# Show file contents (confirm actual reading of .rs file)
run_test "./build/twee --show rs --only tests/dir"

echo -e "\nTests completed. Check $LOG_FILE for details."
