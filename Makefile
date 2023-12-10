CC = gcc
CFLAGS = -Wall -Iinclude
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Get all .c files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate .o filenames in the build directory
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Name of the  executable
TARGET = $(BIN_DIR)/run.out

# Default target
all: $(TARGET)

# Rule to compile each .c file into .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

# link all .o files into the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# clean generated files
clean:
	rm -f $(OBJS) $(TARGET)