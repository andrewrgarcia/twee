# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files
SRCS = main.c config.c filesystem.c display.c ignore_handler.c

# Object files (Replace .c with .o)
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = twee

# Default rule: Compile everything
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compile individual .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
	rm -f $(OBJS) $(TARGET)
