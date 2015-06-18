/*
 * System.cpp
 *
 *  Created on: 14-apr.-2015
 *      Author: Matthias
 */

#include "System.h"

System::System() {
	// TODO Auto-generated constructor stub

}

void System::setGame(Game* game) {
		this->game=game;
}

System::~System() {
	// TODO Auto-generated destructor stub
}

void System::setSystemBit(std::bitset<BITSIZE> bit) {
	systemBit = bit;
}

std::bitset<BITSIZE> System::getSystemBit() {
	return systemBit;
}



std::bitset<BITSIZE> System::getTypeFlags() {
	return typeFlags;
}

void System::addEntity(Entity* entity) {
	entities.push_back(entity);
}

void System::removeEntity(Entity* entity) {
	std::vector<Entity*>::iterator it;
	for(it = entities.begin(); it != entities.end();) {
	        if(entity == (*it)) {
	        	it = entities.erase(it);
	        }
	        else {
	        	it++;
	        }
	 }

}

void System::process() {
	std::vector<Entity*>::iterator it;
		for(it = entities.begin(); it != entities.end(); ++it) {
				processEntity((*it));
		}
}
