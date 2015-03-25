//============================================================================
// Name        : Game.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <SDL2/SDL.h>

#include "Game.h"



#include "Settings.h"
//#include "SharedResources.h"
//#include "game.h"
using namespace std;

std::string parseArg(const std::string &arg) {
	std::string result = "";

	// arguments must start with '--'
	if (arg.length() > 2 && arg[0] == '-' && arg[1] == '-') {
		for (unsigned i = 2; i < arg.length(); ++i) {
			if (arg[i] == '=') break;
			result += arg[i];
		}
	}

	return result;
}

std::string parseArgValue(const std::string &arg) {
	std::string result = "";
	bool found_equals = false;

	for (unsigned i = 0; i < arg.length(); ++i) {
		if (found_equals) {
			result += arg[i];
		}
		if (arg[i] == '=') found_equals = true;
	}

	return result;
}

int main(int argc, char* argv[]) {
bool debug_event = false;
	bool done = false;
	std::string render_device_name = "";

	for (int i = 1 ; i < argc; i++) {
		std::string arg = std::string(argv[i]);
		if (parseArg(arg) == "debug-event") {
			debug_event = true;
		}
		else if (parseArg(arg) == "data-path") {
			CUSTOM_PATH_DATA = parseArgValue(arg);
			if (!CUSTOM_PATH_DATA.empty() && CUSTOM_PATH_DATA.at(CUSTOM_PATH_DATA.length()-1) != '/')
				CUSTOM_PATH_DATA += "/";
		}
		else if (parseArg(arg) == "version") {
			printf("%s\n", getVersionString().c_str());
			done = true;
		}
		else if (parseArg(arg) == "renderer") {
			render_device_name = parseArgValue(arg);
		}
		else if (parseArg(arg) == "help") {
			printf("\
--help                   Prints this message.\n\n\
--version                Prints the release version.\n\n\
--data-path=<PATH>       Specifies an exact path to look for mod data.\n\n\
--debug-event            Prints verbose hardware input information.\n\n\
--renderer=<RENDERER>    Specifies the rendering backend to use.\n\
						 The default is 'sdl'.\n");
			done = true;
		}
	}

	if (!done) {
		srand((unsigned int)time(NULL));
//		try {
				Game *game1 = new Game(render_device_name);
				game1->start();
				delete game1;
//			}
//			catch (const std::runtime_error& e) {
//				/*try software rendering*/
//				cout << "fault with initialisation of SDL GPU rendering \n";
//			}
	}
	return 0;
	//createContext
}