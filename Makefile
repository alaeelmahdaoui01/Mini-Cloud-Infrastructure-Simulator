# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Target executable
TARGET = simulator

# Source files
SRC = src/main.cpp \
      src/Cluster.cpp \
      src/Server.cpp \
      src/Pod.cpp \
      src/Container.cpp \
      src/Resource.cpp \
      src/LeastLoadedScheduler.cpp \
      src/RoundRobinScheduler.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Default rule
all: $(TARGET)

# Link object files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET)
# 	on windows : 
# 	del /f $(OBJ) $(TARGET) 


