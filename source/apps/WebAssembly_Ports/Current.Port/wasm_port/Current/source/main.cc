#include"main_window.h"
#include<iostream>
#include<cstdlib>
#include<ctime>

int main(int argc, char **argv) {

	try {
		srand ( static_cast<unsigned int>(time(0)) );
		//mx::mxTTF_Font::Init();
		current::mainWindow mw;
		return mw.messageLoop();
	} catch(mx::mxException<std::string> &e) {
		e.printError(std::cerr);
	} catch( std::exception &e ) {
		std::cerr << e.what() << "\n";
	} catch( ... ) {
		std::cerr << "unknown error\n";
	}

	return EXIT_FAILURE;
}
