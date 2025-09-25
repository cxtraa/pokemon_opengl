# Compiler and settings
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -MMD -MP
LDFLAGS := -lglfw -lglew -framework OpenGL

# Target name
TARGET := main

# Directories
SRC_DIR := src
OBJ_DIR := obj

# All source files and corresponding object + dep files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)

# Default rule
all: $(TARGET)

# Link final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile each .cpp to .o (and generate .d automatically)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create obj dir if missing
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Include dependency files if they exist
-include $(DEPS)