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

echo -e "\nTests completed. Check $LOG_FILE for details."
