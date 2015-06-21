/*
 * Game.h
 *
 *  Created on: 9-mrt.-2015
 *      Author: Matthias
 */

#ifndef GAME_H_
#define GAME_H_

/*#include "AbstractFactory.h"
#include "SDLGpuFactory.h"*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <vector>

#include "Renderer.h"
#include "EntityFactory.h"
#include "TypeInfoComparator.h"
#include "SystemManager.h"
#include "BitSize.h"




class System;
class Entity;


class Game {

public:
	Game(std::string &render_device_name, int screenWidth, int screenHeight);
	Entity* createEntity();

	void start();
	virtual ~Game();
	const float getDelta() const;

	std::map<int, std::vector<IComponent*> *>& getComponentsByType();
	void printComponentsByType();
	void updateEntity(Entity& entity);
	void printEntityInfo(Entity *entity);
	void printEntityInfo();
	void refreshSystemComponentLinks(Entity *entity);
	void refreshSystemComponentLinks();
	const Rect& getScreen() const;
	SystemManager* getSystemManager();

private:
	SystemManager * systemManager;

	EntityFactory * factory;


	std::vector<Entity *> activeEntityList;
	std::vector<Entity *> removedAndAvailableEntityList;
	std::map<int, std::vector<IComponent*>*> componentsByType;


	std::vector<Entity*> entitiesToUpdate;
	uint32_t nextAvailableId;
	uint32_t count;
	long uniqueEntityId;
	long totalCreated;
	long totalRemoved;
	const float delta = 0.1;
	Entity * playerCar;
	Entity * backgrounds;


	Rect screen;
};




#endif /* GAME_H_ */
