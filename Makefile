# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Output file name
TARGET = options_trading_simulator

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source files (add your MVC components here)
SRC_FILES = main.cpp \
            $(SRC_DIR)/BlackScholes.cpp \
            $(SRC_DIR)/Option.cpp \
            $(SRC_DIR)/Trade.cpp \
            $(SRC_DIR)/FileManager.cpp \
            $(SRC_DIR)/Market.cpp \
            $(SRC_DIR)/Utility.cpp \
            $(SRC_DIR)/Simulation.cpp \
            $(SRC_DIR)/View.cpp \
            $(SRC_DIR)/Command.cpp \
            $(SRC_DIR)/Controller.cpp \
            $(SRC_DIR)/Pool.cpp

# Object files
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files into the final executable
$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(TARGET)

# Clean up object files and the executable
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Rebuild the project
rebuild: clean $(TARGET)

.PHONY: clean rebuild
