# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g -Iinclude

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Find all source files in src/
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Convert each .c file in src/ to a corresponding .o file in build/
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Executable name
TARGET = $(BUILD_DIR)/twee

# Default rule: Compile everything
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)  # Ensure build/ exists
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compile each .c file into an .o file inside build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)  # Ensure build/ exists
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
	rm -rf $(BUILD_DIR)
