
/*
 * Game.cpp
 *
 *  Created on: 9-mrt.-2015
 *      Author: Matthias
 */

#include "Game.h"

#include "Settings.h"
//#include "SharedResources.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "SdlEntityFactory.h"
#include <vector>
#include "GameRenderer.h"
#include <algorithm>
#include "../systems/MovementSystem.h"
#include "../systems/SdlRenderSystem.h"
#include "../systems/SdlInputSystem.h"
#include "../systems/PlayerSpeedSystem.h"
#include "../systems/EnemyCarControlSystem.h"
#include "../systems/backgroundRenderSystem.h"
#include "../systems/roadLaneSystem.h"
#include "../systems/CarLaneMovingSystem.h"
#include "../systems/PlayerCarLaneMovingSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/SdlMovingEntityDebugSystem.h"
#include "../systems/SdlInfoRenderSystem.h"
#include "../systems/DamageSystem.h"
#include "../systems/EnemyCarPlacementSystem.h"
#include <string>
#include <typeinfo>
#include "GameOptions.h"





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

	// SDL Inits
	if ( SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0 ) {
		logError("main: Could not initialize SDL: %s", SDL_GetError());
		exit(1);
	}

	// Create render Device and Rendering Context.
	renderDevice = getRenderDevice(render_device_name);
	int status = renderDevice->createContext(screenWidth, screenHeight);
	factory = new SdlEntityFactory();
	systemManager = new SystemManager(*this);


	movementSystem = systemManager->setSystem(new MovementSystem());
	renderSystem = systemManager->setSystem(new SdlRenderSystem());
	sdlInputSystem = systemManager->setSystem(new SdlInputSystem());
	playerSpeedSystem = systemManager->setSystem(new PlayerSpeedSystem());
	sdlInfoRenderSystem = systemManager->setSystem(new SdlInfoRenderSystem());
	enemyCarControlSystem = systemManager->setSystem(new EnemyCarControlSystem());
	backgroundRenderSystem = systemManager->setSystem(new BackgroundRenderSystem());
	roadLaneSystem = systemManager->setSystem(new RoadLaneSystem());
	playerCarLaneMovingSystem = systemManager->setSystem(new PlayerCarLaneMovingSystem());
	carLaneMovingSystem = systemManager->setSystem(new CarLaneMovingSystem());
	collisionSystem = systemManager->setSystem(new CollisionSystem());
	sdlMovingEntityDebugSystem = systemManager->setSystem(new SdlMovingEntityDebugSystem());
	damageSystem = systemManager->setSystem(new DamageSystem());
	enemyCarPlacementSystem = systemManager->setSystem(new EnemyCarPlacementSystem());



	/*create Game entities and add them to the list of all entities */
	factory->createRoadBorder(this, renderDevice);
	factory->createRoad(this, renderDevice);
	factory->createPlayerCar(this, renderDevice);
	for (int position = 0; position < screen.h*CAR_REPITITION; position +=screen.h) {
		factory->createEnemyCar(this, renderDevice , position);
	}

	//Entity * enemyCar2 = factory->createEnemyCar(this, renderDevice);

	systemManager->initAllSystems();


	refreshSystemComponentLinks();
	printEntityInfo();


	if (status == -1) {

		logError("main: Error during SDL_SetVideoMode: %s", SDL_GetError());
		SDL_Quit();
	}
}

bool priocompare(Entity e1, Entity e2) {
			return e1.getPrio() < e2.getPrio();
		}

void Game::start() {
	bool done = false;
	int delay = int(floor((1000.f/MAX_FRAMES_PER_SEC)+0.5f));

	while ( !done ) {
		int now_ticks = SDL_GetTicks();
		int prev_ticks = now_ticks;

		renderDevice->blankScreen();





		//process each system with his own entity list just build up
		sdlInputSystem->process();
		enemyCarControlSystem->process();
		carLaneMovingSystem->process();
		playerSpeedSystem->process();
		//always do collisionsystem after speed and before movent to prevent speed changes before lock of speed which are not represented in SpeedOfImpact
		collisionSystem->process();
		movementSystem->process();
		backgroundRenderSystem->process();
		renderSystem->process();
		sdlInfoRenderSystem->process();
		roadLaneSystem->process();

		playerCarLaneMovingSystem->process();

		sdlMovingEntityDebugSystem->process();
		damageSystem->process();
		enemyCarPlacementSystem->process();

		//gameRenderer->render(renderList);
		//road->advance();

		// display the FPS counter
		// if the frame completed quickly, we estimate the delay here
		now_ticks = SDL_GetTicks();
		int delay_ticks = 0;
		if (now_ticks - prev_ticks < delay) {
			delay_ticks = delay - (now_ticks - prev_ticks);
		}
		if (now_ticks+delay_ticks - prev_ticks != 0) {
			//gswitch->showFPS(1000 / (now_ticks+delay_ticks - prev_ticks));
		}

		renderDevice->commitFrame();

		// delay quick frames
		now_ticks = SDL_GetTicks();
		if (now_ticks - prev_ticks < delay) {
			SDL_Delay(delay - (now_ticks - prev_ticks));
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
	/*delete gswitch;

	delete anim;
	delete comb;
	delete font;
	delete inpt;
	delete mods;
	delete msg;
	delete snd;
	delete curs;*/

	//Mix_CloseAudio();

	if (renderDevice)
		renderDevice->destroyContext();
	delete renderDevice;

	SDL_Quit();
}



Renderer* Game::getRenderer() const {
	return renderDevice;
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
