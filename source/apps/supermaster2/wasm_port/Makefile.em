all:
	em++ -DFOR_WASM pixelbuffer.cpp mxfont.cpp MasterMain.cpp -o supermaster2.html -sUSE_SDL=2 --preload-file assets -Wno-narrowing -Wno-write-strings -Wno-return-type