/*
 * SdlEntityFactory.cpp
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#include "SdlEntityFactory.h"

/** all components used */
#include "../components/PositionComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/SdlRenderComponent.h"
#include "../components/BackgroundTilingComponent.h"
#include "../components/InputComponent.h"
#include "../components/RoadComponent.h"
#include "../components/LaneComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/SdlDebugComponent.h"
#include "../components/DamageComponent.h"
#include "../components/SdlInfoRenderComponent.h"
#include "../components/AICarComponent.h"



/** other includes */
#include "game.h"
#include <random>
#include "Textures.h"
#include "GameOptions.h"


SdlEntityFactory::SdlEntityFactory(Game* game) {

	// Create render Device and Rendering Context.

	renderDevice = new SdlHardwareRenderer();
	Rect screen = game->getScreen();
	int status = renderDevice->createContext(screen.w, screen.h);

	this->systemManager = game->getSystemManager();


	//create all systems
	movementSystem = systemManager->setSystem(new MovementSystem());
	renderSystem = systemManager->setSystem(new SdlRenderSystem());
	sdlInputSystem = systemManager->setSystem(new SdlInputSystem());
	playerSpeedSystem = systemManager->setSystem(new PlayerSpeedSystem());
	sdlInfoRenderSystem = systemManager->setSystem(new SdlInfoRenderSystem());
	aICarControlSystem = systemManager->setSystem(new AICarControlSystem());
	backgroundTilingSystem = systemManager->setSystem(new BackgroundTilingSystem());
	roadLaneSystem = systemManager->setSystem(new RoadLaneSystem());
	playerCarLaneMovingSystem = systemManager->setSystem(new PlayerCarLaneMovingSystem());
	carLaneMovingSystem = systemManager->setSystem(new CarLaneMovingSystem());
	collisionSystem = systemManager->setSystem(new CollisionSystem());
	sdlMovingEntityDebugSystem = systemManager->setSystem(new SdlMovingEntityDebugSystem());
	damageSystem = systemManager->setSystem(new DamageSystem());
	aICarPlacementSystem = systemManager->setSystem(new AICarPlacementSystem());

	systemManager->initAllSystems();

	renderReferenceComponent = new RenderReferenceComponent(NULL);
}

SdlEntityFactory::~SdlEntityFactory() {
	if (renderDevice)
		renderDevice->destroyContext();
	delete renderDevice;

	SDL_Quit();
}

Entity* SdlEntityFactory::createPlayerCar(Game* game) {
	playerCar = game->createEntity();
	playerCar->addComponent(new PositionComponent(Point(game->getScreen().w/2,0)));
	playerCar->addComponent(new VelocityComponent(0,0));
	SdlRenderComponent * renderComponent = new SdlRenderComponent(renderDevice);
	Rect carTextureBox;

	carTextureBox.w = game->getScreen().w/8;
	carTextureBox.h = carTextureBox.w * 2;
	carTextureBox.x = carTextureBox.w / 2;
	carTextureBox.y = carTextureBox.h/2;//- game->getScreen().h + carTextureBox.h *1.1 ;
	renderComponent->addStaticSpritefromTexture(PATH_PLAYER_CAR,0, carTextureBox, game->getScreen());

	playerCar->addComponent(renderComponent);
	playerCar->addComponent(new InputComponent());
	renderReferenceComponent->setReference(playerCar, Point(0 , -game->getScreen().h*0.9 + carTextureBox.h/2 ));  //set car 10% higher then bottom of window
	playerCar->addComponent(renderReferenceComponent);
	playerCar->addComponent(new SdlInfoRenderComponent(renderDevice,PATH_RACEFONT, game->getScreen().w * 0.04, PATH_STANDARDFONT, game->getScreen().w * 0.03));
	//also add road to playercar
	if (road != NULL) {
		playerCar->addComponent(new LaneComponent(road));
	}
	else {
		std::cerr << "first create road before car" << std::endl;
	}

	Rect collisionBox;
	collisionBox.h = carTextureBox.h;
	collisionBox.w = carTextureBox.w*0.9;
	collisionBox.x = collisionBox.w/2; //is used as offset
	collisionBox.y = collisionBox.h/2;
	playerCar->addComponent(new CollisionComponent(playerCar, collisionBox));
	playerCar->addComponent(new SdlDebugComponent(renderDevice,PATH_STANDARDFONT, carTextureBox.h * 0.05));
	playerCar->addComponent(new DamageComponent());
	playerCar->update();
	return playerCar;
}

Entity* SdlEntityFactory::createEnemyCar(Game* game, int position) {
	static std::default_random_engine generator;
	std::uniform_int_distribution<> random_speed(MIN_ENEMY_CAR_SPEED,MAX_ENEMY_CAR_SPEED);
	Entity * enemyCar = game->createEntity();
	enemyCar->addComponent(new PositionComponent(FPoint(game->getScreen().w/2 + game->getScreen().w/4.5,position)));  //set one lane further
	enemyCar->addComponent(new VelocityComponent(0,random_speed(generator)));
	SdlRenderComponent * renderComponent = new SdlRenderComponent(renderDevice);
	Rect carTextureBox;
	carTextureBox.w = game->getScreen().w/7;
	carTextureBox.h = carTextureBox.w * 2;
	carTextureBox.x = carTextureBox.w / 2;
	carTextureBox.y =  carTextureBox.h/2;
	renderComponent->addStaticSpritefromTexture(getRandomCarTexture(), 0,  carTextureBox, game->getScreen());
	enemyCar->addComponent(renderComponent);
	enemyCar->addComponent(renderReferenceComponent);
	//also add road to enemycar
	if (road != NULL) {
		enemyCar->addComponent(new LaneComponent(road));
	}
	else {
		std::cerr << "first create road before car" << std::endl;
	}
	Rect collisionBox;
	collisionBox.h = carTextureBox.h;
	collisionBox.w = carTextureBox.w*0.9;
	collisionBox.x = collisionBox.w/2; //is used as offset
	collisionBox.y = collisionBox.h/2;
	enemyCar->addComponent(new CollisionComponent(enemyCar, collisionBox));
	enemyCar->addComponent(new AICarComponent(enemyCar));
	enemyCar->addComponent(new SdlDebugComponent(renderDevice,PATH_STANDARDFONT, carTextureBox.h * 0.05));
	enemyCar->addComponent(new DamageComponent());
	enemyCar->update();
	return enemyCar;
}

Entity* SdlEntityFactory::createRoad(Game* game) {
	road = NULL;
	if (checkrenderReferenceComponent()) {
		road = game->createEntity();
		road->addComponent(new PositionComponent(Point(game->getScreen().w/2, 0)));
		road->addComponent(new BackgroundTilingComponent(0, 20, game->getScreen()));
		Rect road3laneTextureBox;
		road3laneTextureBox.w = game->getScreen().w/1.5;
		road3laneTextureBox.h = road3laneTextureBox.w /2;
		road3laneTextureBox.x = road3laneTextureBox.w /2;
		road3laneTextureBox.y =  0;
		SdlRenderComponent * renderComponent = new SdlRenderComponent(renderDevice);
		renderComponent->addStaticSpritefromTexture(PATH_ROAD_STRAIGHT_3_LANES, 0,  road3laneTextureBox, game->getScreen());
		road->addComponent(renderComponent);
		road->addComponent(renderReferenceComponent);
		road->addComponent(new RoadComponent(road3laneTextureBox.w/3, game->getScreen().w/2, 3));
	}
	road->update();
	return road;
}

Entity* SdlEntityFactory::createRoadBorder(Game* game) {

	Rect roadBorderTextureBox;
	roadBorderTextureBox.w = game->getScreen().w/4;
	roadBorderTextureBox.h = roadBorderTextureBox.w;
	roadBorderTextureBox.x = roadBorderTextureBox.w/2;
	roadBorderTextureBox.y =  0;


	Entity * roadBorderLeft = NULL;
	if (checkrenderReferenceComponent()) {
		roadBorderLeft = game->createEntity();
		roadBorderLeft->addComponent(new PositionComponent(Point(game->getScreen().w *0.9, 0)));
		SdlRenderComponent * renderComponentL = new SdlRenderComponent(renderDevice);
		roadBorderLeft->addComponent(new BackgroundTilingComponent(0, 20, game->getScreen()));
		renderComponentL->addStaticSpritefromTexture(PATH_GRASS_FULL, 0, roadBorderTextureBox, game->getScreen());
		roadBorderLeft->addComponent(renderComponentL);
		roadBorderLeft->addComponent(renderReferenceComponent);
	}
	roadBorderLeft->update();
	Entity * roadBorderRight = NULL;
	if (checkrenderReferenceComponent()) {
		roadBorderRight = game->createEntity();
		roadBorderRight->addComponent(new PositionComponent(Point(game->getScreen().w*0.10, 0)));
		SdlRenderComponent * renderComponentR = new SdlRenderComponent(renderDevice);
		roadBorderRight->addComponent(new BackgroundTilingComponent(0, 20, game->getScreen()));
		renderComponentR->addStaticSpritefromTexture(PATH_GRASS_FULL, 0,  roadBorderTextureBox, game->getScreen());
		roadBorderRight->addComponent(renderComponentR);
		roadBorderRight->addComponent(renderReferenceComponent);
	}
	roadBorderRight->update();
	return roadBorderRight;
}


bool SdlEntityFactory::checkrenderReferenceComponent() {
	if (renderReferenceComponent != NULL) {
		return true;
	}
	else {
		std::cerr << "first set renderReferenceComponent to player car";
		return false;
	}
}

std::string SdlEntityFactory::getRandomCarTexture() {
	static std::default_random_engine generator;
	std::uniform_int_distribution<> random_car(0,enemyCars.size()-1);
	return enemyCars[random_car(generator)];
}

void SdlEntityFactory::executeAllSystems() {
	renderDevice->blankScreen();

	//process each system with his own entity list just build up
	sdlInputSystem->process();
	aICarControlSystem->process();
	carLaneMovingSystem->process();
	playerSpeedSystem->process();
	//always do collisionsystem after speed and before movent to prevent speed changes before lock of speed which are not represented in SpeedOfImpact
	collisionSystem->process();
	movementSystem->process();
	backgroundTilingSystem->process();
	renderSystem->process();
	sdlInfoRenderSystem->process();
	roadLaneSystem->process();

	playerCarLaneMovingSystem->process();

	sdlMovingEntityDebugSystem->process();
	damageSystem->process();
	aICarPlacementSystem->process();


	renderDevice->commitFrame();
}
