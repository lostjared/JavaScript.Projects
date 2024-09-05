# Makefile for building with em++ and SDL2, creating a static library with emar

# Compiler, archiver, and flags
CXX = em++
AR = emar
CXXFLAGS = -std=c++98 -sUSE_SDL=1 -I/home/jared/emscripten-libs/jpeg/include -I/home/jared/emscripten-libs/libpng/include -I/home/jared/emscripten-libs/zlib/include -DNOGL

# Source files and target
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = libmx.a

# Build rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build rule for the archive
$(TARGET): $(OBJ)
	$(AR) rcs $(TARGET) $(OBJ)

# Clean rule
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean
