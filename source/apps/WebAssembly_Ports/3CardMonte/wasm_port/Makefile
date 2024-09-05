### be sure to set SDL_PATH to the path of SDL2

CXX=em++
$SDL_PATH=/opt/local/sdl
CPP_FLAGS= -std=c++11 #-stdlib=libc++ -Wall -sUSE_SDL=2 -sUSE_SDL_TTF=2
CXX_FLAGS= #-I/usr/include -I/usr/local/include  `sdl2-config --cflags`
LD_FLAGS= -sUSE_SDL=2 -sUSE_SDL_TTF=2 #`freetype-config --libs` `sdl2-config --static-libs` -lSDL2_ttf

all:
	$(CXX) $(CPP_FLAGS) 3card.cpp -c -o 3card.o $(CXX_FLAGS)
	$(CXX) $(CPP_FLAGS) 3card.o -o 3card.html --preload-file assets $(LD_FLAGS)
clean:
	rm -f 3card-prog *.o
