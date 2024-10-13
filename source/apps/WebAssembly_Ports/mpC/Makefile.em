CC=emcc

CFLAGS= -s USE_SDL=2 -s USE_SDL_TTF=2 
LDFLAGS= -s USE_SDL=2 -s USE_SDL_TTF=2 --preload-file data
CPP_FILES := $(wildcard *.c)
OBJ_FILES := $(addprefix ,$(notdir $(CPP_FILES:.c=.o)))

source/%.o: source/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
  
all: $(OBJ_FILES) 
	$(CC) $(OBJ_FILES)  -o mpc.html  $(LDFLAGS)

clean:
	rm -f *.o 
