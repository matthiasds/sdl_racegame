
/*
 * Game.cpp
 *
 *  Created on: 9-mrt.-2015
 *      Author: Matthias
 */

#include "Game.h"

//#include "SharedResources.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "SdlEntityFactory.h"
#include <vector>
#include <algorithm>

#include <string>
#include <typeinfo>
#include "GameOptions.h"
#include <chrono>
#include <windows.h>





/**
 * Game initialization.
 */

Game::Game(std::string &render_device_name, int screenWidth, int screenHeight) {

	nextAvailableId = 0;
	count = 0;
	uniqueEntityId = 0;
	totalCreated = 0;
	totalRemoved = 0;

	screen.w = screenWidth;
	screen.h = screenHeight;

	systemManager = new SystemManager(*this);

	//create specific factory based on the render_device_name argument of the program
	if (render_device_name != "") {
		if (render_device_name == "sdl") {
			std::cerr << "no software renderer support yet";
		}
		else if (render_device_name == "sdl_hardware") {
			factory = new SdlEntityFactory(this);
		}
		else {
				std::cerr << "Render device " << render_device_name.c_str();
		}
	}
	else {
		std::cerr << "illegal render argument";
	}


	/*create Game entities and add them to the list of all entities */
	factory->createRoadBorder(this);
	factory->createRoad(this);
	factory->createPlayerCar(this);
	for (int position = 0; position < screen.h*CAR_REPITITION; position +=screen.h/2) {
		factory->createEnemyCar(this, position);
	}

	refreshSystemComponentLinks();
	printEntityInfo();


//	if (status == -1) {
//
//		std::cerr << "main: Error during SDL_SetVideoMode: %s" << SDL_GetError();
//		SDL_Quit();
//	}
}

bool priocompare(Entity e1, Entity e2) {
			return e1.getPrio() < e2.getPrio();
		}

void Game::start() {
	bool done = false;
	std::chrono::milliseconds frameDelay(int(floor((1000.f/MAX_FRAMES_PER_SEC)+0.5f)));


	while ( !done ) {

		std::chrono::time_point<std::chrono::system_clock> now_ticks = std::chrono::system_clock::now();
		std::chrono::time_point<std::chrono::system_clock> prev_ticks = now_ticks;


		factory->executeAllSystems();



		now_ticks = std::chrono::system_clock::now();

		if (now_ticks - prev_ticks < frameDelay) {
			std::chrono::duration<double> timeToSleep = frameDelay - (now_ticks - prev_ticks);
			Sleep((std::chrono::duration_cast<std::chrono::milliseconds>(timeToSleep)).count());
		}
	}
}

void Game::refreshSystemComponentLinks() {
	std::vector<Entity*>::iterator currentEntity = entitiesToUpdate.begin();
	std::vector<Entity*>::iterator lastEnitty = entitiesToUpdate.end();

	//loop through all active entities
	for (;currentEntity != lastEnitty; ++currentEntity) {
		refreshSystemComponentLinks((*currentEntity));
	}
}


void Game::refreshSystemComponentLinks(Entity *entity) {
	std::vector<System*> systems = systemManager->getSystems(); //get all the systems in the game
	std::vector<System*>::iterator currentSystem;
	// update the system component matches
	for(currentSystem=systems.begin(); currentSystem != systems.end(); ++currentSystem) { //check all systems
		std::bitset<BITSIZE>  currentSystem_SystemBit = (*currentSystem)->getSystemBit(); //get the bit representing the system
		std::bitset<BITSIZE>  currentSystem_TypeFlags = (*currentSystem)->getTypeFlags(); //get the flags representing the current systems components

		//check if entity already is subscribed to this system
		bool contains = (currentSystem_SystemBit & entity->getSystemBits()) == currentSystem_SystemBit;
		//check if the entity has all components to be able to subscribe to this system
		bool interest = (currentSystem_TypeFlags & entity->getTypeBits()) == currentSystem_TypeFlags;

		//if the entity has all components to subscribe but has'nt subscribed yet
		if(interest && !contains && currentSystem_TypeFlags.any()) {
			(*currentSystem)->addEntity(entity); //add entity to the system
			entity->addSystemBit(currentSystem_SystemBit); //update entity with systembit
		//if the entity has not the components to be subscribed to the system but is subscribed
		} else if(!interest && contains && currentSystem_TypeFlags.any()) {
			(*currentSystem)->removeEntity(entity);
		}
	 }
}

void Game::updateEntity(Entity& entity) {
	entitiesToUpdate.push_back(&entity);
	}

void Game::printEntityInfo() {
	std::vector<Entity*>::iterator currentEntity = entitiesToUpdate.begin();
	std::vector<Entity*>::iterator lastEnitty = entitiesToUpdate.end();

	for (;currentEntity != lastEnitty; ++currentEntity) {
				printEntityInfo(*currentEntity);
	}
	printComponentsByType();
}


void Game::printEntityInfo(Entity *entity) {
	//get all the systems in the game
	std::vector<System*> gameSystems = systemManager->getSystems();
	std::vector<System*>::iterator currentSystem; // create iterator for iterating the systems
	std::cout << "Entity id: " << entity->getId() << ", Entity unique-id: " << entity->getUniqueId() << " contains these Systems: " << std::endl;
	//get the systems of the entity
	std::bitset<BITSIZE> entitySystemBits = entity->getSystemBits();

	// iterate all existing systems
	for(currentSystem=gameSystems.begin(); currentSystem != gameSystems.end(); ++currentSystem) { //check all systems
		//get the systemBit representing the current system
		std::bitset<BITSIZE>  currentSystem_SystemBit = (*currentSystem)->getSystemBit();
		//get the TypeFlags representing the current systems components
		std::bitset<BITSIZE>  currentSystem_TypeFlags = (*currentSystem)->getTypeFlags();

		//check if entity already is subscribed to this system
		if ((currentSystem_SystemBit & entitySystemBits) == currentSystem_SystemBit) {
			std::cout << "\t System  " << (uint32_t) currentSystem_SystemBit.to_ulong() << " AKA" << typeid((*(*currentSystem))).name() << " is subscribed to following components: " << std::endl;

			//list all kown components and check to which the system i subscribed
			for (auto const &currentComponentEntityList : componentsByType) {
				IComponent* firstEntitityInListComponent = NULL;
				if ((int) (currentComponentEntityList.second)->size() - 1 >= entity->getId()) {  //if id position is available in component vector
					firstEntitityInListComponent = (currentComponentEntityList.second)->at(entity->getId());
				}
				if (firstEntitityInListComponent != NULL) {
					ComponentType firstEntitityInListComponentType = ComponentTypeManager::getTypeFor(typeid(*firstEntitityInListComponent));
					std::bitset<BITSIZE> firstEntitityInListComponentBit = firstEntitityInListComponentType.getBit();
					//check if the system contains this component
					if ((currentSystem_TypeFlags & firstEntitityInListComponentBit) != 0) {
						std::cout << "\t\tComponent " << (currentComponentEntityList.first) << " of map of type " << firstEntitityInListComponentType.getId() << " AKA " << typeid(*firstEntitityInListComponent).name() << std::endl;
					}
				}
			}
		}



	 }
	std::cout << "----------------------------------------------- " << std::endl;

}


Game::~Game() {
	std::cout << "Game Destructor callled \n";
}


const float Game::getDelta() const {
	return delta;
}

Entity* Game::createEntity() {
	Entity * entity = NULL;
	if (removedAndAvailableEntityList.size()>0)
	{
		 entity = this->removedAndAvailableEntityList.back();
		this->removedAndAvailableEntityList.pop_back();
	}
	if(entity == NULL) { // can't reuse existing entity, create new one
		entity = new Entity(this, nextAvailableId++);
	}
	else { //reuse existing entity
		entity->reset(); //reset component and system id bits
	}
	entity->setUniqueId(uniqueEntityId++);
	activeEntityList.push_back(entity);
	count++;
	totalCreated++;
	return entity;
}

std::map<int, std::vector<IComponent*> *>& Game::getComponentsByType(){
	return componentsByType;
}

void Game::printComponentsByType() {
	int nrOfElements = componentsByType.size();
	std::vector<IComponent*> * mapValue;
	int mapKey;
	std::cout << "ComponentsByType is map of " << nrOfElements << " Keys: " << std::endl;
	for (auto const &it : componentsByType) {
		mapValue = it.second;
		mapKey= it.first;
		int nrOfComponents = mapValue->size();
		std::cout << "Key " << mapKey << " contains these components: " << std::endl;
		for (int componentIndex=0; componentIndex < nrOfComponents; componentIndex++){
			IComponent* component = mapValue->at(componentIndex);
			if (component != NULL)
			{
			ComponentType type = ComponentTypeManager::getTypeFor(typeid(*component));
			std::cout << "\t Component for entity (id)" << componentIndex << " is type " << type.getId() << " AKA " << typeid(*component).name() << std::endl;
			}
		}
	}
	std::cout << "----------------------------------------------- " << std::endl;

}

const Rect& Game::getScreen() const {
	return screen;
}

SystemManager* Game::getSystemManager(){
	return systemManager;
}

