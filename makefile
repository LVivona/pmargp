# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra

# Project directories
SRC_DIR = src
TEST_DIR = test
BIN_DIR = bin
EXAMPLE_DIR = example

# Source files
LIB_SRC = $(SRC_DIR)/pmargp.c
LIB_HEADER = $(SRC_DIR)/pmargp.h
TEST_SRC = $(TEST_DIR)/test.c
EXAMPLE_SRC = $(EXAMPLE_DIR)/example.c

# Object and executable files in the bin directory
LIB_OBJ = $(BIN_DIR)/pmargp.o
EXECUTABLE = $(BIN_DIR)/test
EXAMPLE_EXECUTABLE = $(BIN_DIR)/example_program

# Ensure the bin directory exists before compiling
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile the object file for the library
$(LIB_OBJ): $(LIB_SRC) $(LIB_HEADER) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $(LIB_SRC) -o $(LIB_OBJ)

# Build the test executable, ensuring the compiler looks in the src directory for the header
$(EXECUTABLE): $(LIB_OBJ) $(TEST_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(LIB_OBJ) $(TEST_SRC) -o $(EXECUTABLE)

# Build the example executable
$(EXAMPLE_EXECUTABLE): $(LIB_OBJ) $(EXAMPLE_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(LIB_OBJ) $(EXAMPLE_SRC) -o $(EXAMPLE_EXECUTABLE)

# Run the test
test: $(EXECUTABLE)
	@if ./$(EXECUTABLE) --all; then \
	    echo "Test passed. Removing test executable."; \
	    rm -f $(EXECUTABLE); \
	else \
	    echo "Test failed. Removing both test executable and object file."; \
	    rm -f $(EXECUTABLE) $(LIB_OBJ); \
	fi

# Clean up (remove bin directory and all its contents)
clean:
	rm -rf $(BIN_DIR)

example: $(EXAMPLE_EXECUTABLE)
	@echo "done"

all: $(EXAMPLE_EXECUTABLE) $(LIB_OBJ) $(TEST_SRC) | $(BIN_DIR)

.PHONY: all clean