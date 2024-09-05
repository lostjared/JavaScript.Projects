CXX=em++
OBJ=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

CFLAGS= -std=c++98 -sUSE_SDL=2 -sUSE_SDL_TTF=2
LDFLAGS= -sUSE_SDL=2 -sUSE_SDL_TTF=2 --preload-file assets
PROGRAM=lostmasterpiece.html

%.o: %.cpp
	$(CXX) $(CFLAGS) $< -c -o $@

$(PROGRAM): $(OBJ)
	$(CXX) -o $(PROGRAM)  $(OBJ) $(LDFLAGS)

all: $(PROGRAM)

.PHONY: clean clear
clean:
	rm -f $(PROGRAM)  *.o


