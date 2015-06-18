/*
 * SystemManager.cpp
 *
 *  Created on: 14-apr.-2015
 *      Author: Matthias
 */

#include "SystemManager.h"
#include "SystemBitManager.h"

SystemManager::SystemManager(Game &game) {
	this->game = &game;
}

SystemManager::~SystemManager() {

}

void SystemManager::initAllSystems() {
  std::vector<System*>::iterator currentSystem;
  for(currentSystem=systems.begin(); currentSystem!= systems.end(); ++currentSystem) {
	  (*currentSystem)->init();
  }
}

System* SystemManager::setSystem(System* system) {
	bool bitFlag = false;
	int index = 0;

	//Check if system is known.
	 std::vector<System*>::iterator currentSystem;
	 std::vector<System*>::iterator matchedSystem;
	 for(currentSystem=systems.begin(); currentSystem!= systems.end(); ++currentSystem) {
	  if(typeid(*system) == typeid(*(*currentSystem))){
		bitFlag = true;
		matchedSystem = currentSystem;
	  }
	}

	//if no existing system match is found in previous for loop
	if (!bitFlag) {
		system->setGame(game);  //add new system to systems
		systemsByType[&typeid(*system)]  = system;
		systems.push_back(system);
		system->setSystemBit(SystemBitManager::getBitFor(typeid(*system)));
	}
	else if(systems[index] != system){// existing system found end system pointer does'nt point to an existing system
	//Check if system pointer doesn't point to an existing system
	//Else add system to manager
	  //If it doesn't point to an existing system
		//Delete newly made system.
		delete system;
		//Point to existing system in bag.
		system = (*matchedSystem);
	 }

	return system;
}

std::vector<System*> & SystemManager::getSystems() {
	return systems;
}



