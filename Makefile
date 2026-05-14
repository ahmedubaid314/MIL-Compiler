# Compiler project Makefile
# Builds the MIL compiler executable
# Auto-detects platform (Linux/Windows)

# Detect OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    PLATFORM := linux
endif
ifeq ($(UNAME_S),Darwin)
    PLATFORM := macos
endif
ifneq ($(filter MINGW%,$(UNAME_S)),)
    PLATFORM := windows
endif
ifneq ($(filter MSYS%,$(UNAME_S)),)
    PLATFORM := windows
endif

# If auto-detection fails, allow manual override
PLATFORM ?= linux

# Directories
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include

# Compiler and tools
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I$(INCLUDE_DIR)

# Source files
SOURCES := main.cpp $(SRC_DIR)/scanner.cpp $(SRC_DIR)/parser.cpp
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))
COMPILER_EXEC := $(BUILD_DIR)/compiler

# Phony targets
.PHONY: all clean help

# Default target
all: $(COMPILER_EXEC)

help:
	@echo "MIL Compiler Makefile"
	@echo "====================="
	@echo "Detected platform: $(PLATFORM)"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build the compiler executable"
	@echo "  make clean    - Remove build directory"
	@echo "  make help     - Show this message"
	@echo ""
	@echo "Usage after build:"
	@echo "  ./$(COMPILER_EXEC) <file.mil>"
	@echo ""
	@echo "To override platform detection, use: make PLATFORM=<linux|windows>"

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile C++ sources to object files
$(BUILD_DIR)/main.o: main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/scanner.o: $(SRC_DIR)/scanner.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/parser.o: $(SRC_DIR)/parser.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link compiler executable
$(COMPILER_EXEC): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Verbose output for debugging
print-platform:
	@echo "Platform: $(PLATFORM)"
	@echo "Compiler executable: $(COMPILER_EXEC)"
