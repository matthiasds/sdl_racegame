/*
 * Utils.h
 *
 *  Created on: 21-jun.-2015
 *      Author: Matthias
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "GameOptions.h"
#include <iomanip>
#include <sstream>

/*using singleton pattern to acces global options like debugging*/
class Utils {
private:
	bool debug_status;
	Utils() {debug_status = false;}

	Utils(Utils const&) = delete;
	void operator=(Utils const&) = delete;
public:
	static Utils& getInstance() {
		static Utils  instance;
		return instance;
	}

	bool isDebuggingEnabled() {
		return debug_status;
	}
	void enableDebugging() {
		debug_status = true;
	}
	void disableDebugging() {
		debug_status = false;
	}

	/**
	 * Return a string of version name + version number
	 */
	std::string getVersionString() {
		std::stringstream ss;
		ss << VERSION_NAME << " v" << VERSION_MAJOR << "." << std::setfill('0') << std::setw(2) << VERSION_MINOR;
		return ss.str();
	}

};


extern Utils utils;


#endif /* UTILS_H_ */
