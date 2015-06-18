/*
 * SystemManager.h
 *
 *  Created on: 14-apr.-2015
 *      Author: Matthias
 */

#ifndef SYSTEMMANAGER_H_
#define SYSTEMMANAGER_H_

#include <vector>
#include <map>
#include "System.h"
#include "TypeInfoComparator.h"

class Game;

class SystemManager {
public:
	SystemManager(Game &game);
	void initAllSystems();
	std::vector<System*> & getSystems();
	System* setSystem(System* system);
	virtual ~SystemManager();
private:
	Game * game;
	std::vector<System*> systems;
	std::map<const std::type_info*, System*, type_info_comparator> systemsByType;
};

#endif /* SYSTEMMANAGER_H_ */
