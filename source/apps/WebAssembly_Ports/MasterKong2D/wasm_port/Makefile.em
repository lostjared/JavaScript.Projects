all:
	em++ -DFOR_WASM start.cpp about.cpp  game.cpp  gameover.cpp  gintro.cpp  grandma.cpp  hero.cpp  intro.cpp  mastermain.cpp  mxfont.cpp  options.cpp  ParticleHandler.cpp  pixelbuffer.cpp  soundhandle.cpp   -o masterkong.html -sUSE_SDL=2 --preload-file assets -Wno-narrowing -Wno-write-strings -Wno-return-type
clean:
	rm -f *.html *.js *.wasm *.data    
