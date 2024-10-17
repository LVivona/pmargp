# Makefile for Poor Man's Argument Parser (pmargp)

# Compiler and flags
CC := gcc
CFLAGS ?= -Wall -Wextra -fPIC  # Added -fPIC here

# Detect OS
UNAME_S := $(shell uname -s)

# Project directories
SRC_DIR := src
TEST_DIR := test
BIN_DIR := bin
EXAMPLE_DIR := example
LIB_DIR := lib

# Library name and version
LIB_NAME := pmargp
VERSION := 0.1.0
MAJOR_VERSION := $(shell echo $(VERSION) | cut -d. -f1)

# Source files
LIB_SRC := $(SRC_DIR)/$(LIB_NAME).c
LIB_HEADER := $(SRC_DIR)/$(LIB_NAME).h
TEST_SRC := $(TEST_DIR)/test.c
EXAMPLE_SRC := $(EXAMPLE_DIR)/example.c

# Object and executable files
LIB_OBJ := $(BIN_DIR)/$(LIB_NAME).o
STATIC_LIB := $(LIB_DIR)/lib$(LIB_NAME).a
TEST_EXECUTABLE := $(BIN_DIR)/test
EXAMPLE_EXECUTABLE := $(BIN_DIR)/example_program

# Installation directories
PREFIX := /usr/local
INSTALL_INC_DIR := $(PREFIX)/include
INSTALL_LIB_DIR := $(PREFIX)/lib

# OS-specific settings
ifeq ($(UNAME_S),Darwin)
	SHARED_LIB := $(LIB_DIR)/lib$(LIB_NAME).$(VERSION).dylib
	SHARED_LIB_LINK := $(LIB_DIR)/lib$(LIB_NAME).dylib
	SHARED_FLAG := -dynamiclib
else
	SHARED_LIB := $(LIB_DIR)/lib$(LIB_NAME).so.$(VERSION)
	SHARED_LIB_LINK := $(LIB_DIR)/lib$(LIB_NAME).so
	SHARED_FLAG := -shared
endif

# Phony targets
.PHONY: all clean test install uninstall

# Default target
all: $(STATIC_LIB) $(SHARED_LIB) $(TEST_EXECUTABLE) $(EXAMPLE_EXECUTABLE)

# Ensure directories exist
$(BIN_DIR) $(LIB_DIR):
	mkdir -p $@

# Compile the object file for the library
$(LIB_OBJ): $(LIB_SRC) $(LIB_HEADER) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $(LIB_SRC) -o $@

# Create static library
$(STATIC_LIB): $(LIB_OBJ) | $(LIB_DIR)
	ar rcs $@ $<

# Create shared library
$(SHARED_LIB): $(LIB_OBJ) | $(LIB_DIR)
	$(CC) $(SHARED_FLAG) -o $@ $<
	ln -sf $(notdir $(SHARED_LIB)) $(SHARED_LIB_LINK)

# Build the test executable
$(TEST_EXECUTABLE): $(TEST_SRC) $(STATIC_LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $< $(STATIC_LIB) -o $@

# Build the example executable
$(EXAMPLE_EXECUTABLE): $(EXAMPLE_SRC) $(STATIC_LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $< $(STATIC_LIB) -o $@

# Run the test
test: $(TEST_EXECUTABLE)
	@if ./$(TEST_EXECUTABLE) --all; then \
		echo "Test passed for $(CFLAGS)"; \
	else \
		echo "Test failed for $(CFLAGS)"; \
		exit 1; \
	fi

# Install the library and header
install: $(STATIC_LIB) $(SHARED_LIB) $(LIB_HEADER)
	install -d $(INSTALL_INC_DIR) $(INSTALL_LIB_DIR)
	install -m 644 $(LIB_HEADER) $(INSTALL_INC_DIR)
	install -m 644 $(STATIC_LIB) $(INSTALL_LIB_DIR)
	install -m 755 $(SHARED_LIB) $(INSTALL_LIB_DIR)
	ln -sf $(notdir $(SHARED_LIB)) $(INSTALL_LIB_DIR)/$(notdir $(SHARED_LIB_LINK))
ifeq ($(UNAME_S),Linux)
	ldconfig
endif

# Uninstall the library and header
uninstall:
	rm -f $(INSTALL_INC_DIR)/$(LIB_NAME).h
	rm -f $(INSTALL_LIB_DIR)/lib$(LIB_NAME).a
	rm -f $(INSTALL_LIB_DIR)/lib$(LIB_NAME).so*
	rm -f $(INSTALL_LIB_DIR)/lib$(LIB_NAME).*.dylib
ifeq ($(UNAME_S),Linux)
	ldconfig
endif

# Clean up
clean:
	rm -rf $(BIN_DIR) $(LIB_DIR)
