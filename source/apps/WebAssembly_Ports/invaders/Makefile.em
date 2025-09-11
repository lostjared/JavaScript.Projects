SRC = sdl.c main.c ship.c projectile.c
OUT = invaders.html
EMCC = emcc
CFLAGS = -O2 -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1
all: $(OUT)
$(OUT): $(SRC)
	$(EMCC) $(SRC) $(CFLAGS) -o $(OUT)
clean:
	rm -f $(OUT) *.js *.wasm *.data

.PHONY: all clean
