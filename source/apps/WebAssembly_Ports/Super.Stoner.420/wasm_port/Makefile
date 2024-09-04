CC=emcc
CXX=em++
SDL_PATH=/opt/local
#CFLAGS=  -I./includes `sdl2-config --cflags` -I. -DHAS_MIXER
#LDFLAGS=`sdl2-config --libs` -lSDL2_mixer
CFLAGS= -sUSE_SDL=2 -sUSE_SDL_MIXER=2 -DHAS_MIXER
LDFLAGS= -sUSE_SDL=2 -sUSE_SDL_MIXER=2 --preload-file assets
CPP_FILES := $(wildcard *.c)
OBJ_FILES := $(addprefix ,$(notdir $(CPP_FILES:.c=.o)))

source/%.o: source/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
  
all: $(OBJ_FILES) 
	$(CXX) $(OBJ_FILES)  -o SuperStoner.html  $(LDFLAGS)

clean:
	rm -f SuperStoner420/SuperStoner *.o 
