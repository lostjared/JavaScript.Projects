# Compiler and flags
CXX = em++
CXXFLAGS = -sUSE_SDL=1 -I/home/jared/emscripten-libs/mx/include -I/home/jared/emscripten-libs/jpeg/include -I/home/jared/emscripten-libs/libpng/include

# Source files and target
SRC = current_game.cc  game_controller.cc  game_screen.cc  main.cc  main_window.cc
OBJ = $(SRC:.cc=.o)
TARGET = Current.html

# Build rule for object files
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

# Link all .o files and produce the HTML
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) -sALLOW_MEMORY_GROWTH --preload-file assets /home/jared/emscripten-libs/mx/lib/libmx.a /home/jared/emscripten-libs/jpeg/lib/libjpeg.a /home/jared/emscripten-libs/libpng/lib/libpng16.a /home/jared/emscripten-libs/zlib/lib/libz.a

# Clean rule
clean:
	rm -f $(OBJ) $(TARGET) *.js *.wasm *.data

.PHONY: clean
