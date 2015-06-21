/*
 * SdlEntityFactory.cpp
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#include "SdlEntityFactory.h"
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
#include "game.h"
#include <random>
#include "Textures.h"
#include "GameOptions.h"


SdlEntityFactory::SdlEntityFactory() {
	renderReferenceComponent = new RenderReferenceComponent(NULL);
}

SdlEntityFactory::~SdlEntityFactory() {
	// TODO Auto-generated destructor stub
}

Entity* SdlEntityFactory::createPlayerCar(Game* game, Renderer* renderer) {
	playerCar = game->createEntity();
	playerCar->addComponent(new PositionComponent(Point(game->getScreen().w/2,0)));
	playerCar->addComponent(new VelocityComponent(0,0));
	SdlRenderComponent * renderComponent = new SdlRenderComponent(renderer);
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
	playerCar->addComponent(new SdlInfoRenderComponent(renderer,PATH_RACEFONT, game->getScreen().w * 0.04, PATH_STANDARDFONT, game->getScreen().w * 0.03));
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
	playerCar->addComponent(new SdlDebugComponent(renderer,PATH_STANDARDFONT, carTextureBox.h * 0.05));
	playerCar->addComponent(new DamageComponent());
	playerCar->update();
	return playerCar;
}

Entity* SdlEntityFactory::createEnemyCar(Game* game, Renderer* renderer, int position) {
	static std::default_random_engine generator;
	std::uniform_int_distribution<> random_speed(MIN_ENEMY_CAR_SPEED,MAX_ENEMY_CAR_SPEED);
	Entity * enemyCar = game->createEntity();
	enemyCar->addComponent(new PositionComponent(FPoint(game->getScreen().w/2 + game->getScreen().w/4.5,position)));  //set one lane further
	enemyCar->addComponent(new VelocityComponent(0,random_speed(generator)));
	SdlRenderComponent * renderComponent = new SdlRenderComponent(renderer);
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
	enemyCar->addComponent(new SdlDebugComponent(renderer,PATH_STANDARDFONT, carTextureBox.h * 0.05));
	enemyCar->addComponent(new DamageComponent());
	enemyCar->update();
	return enemyCar;
}

Entity* SdlEntityFactory::createRoad(Game* game, Renderer* renderer) {
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
		SdlRenderComponent * renderComponent = new SdlRenderComponent(renderer);
		renderComponent->addStaticSpritefromTexture(PATH_ROAD_STRAIGHT_3_LANES, 0,  road3laneTextureBox, game->getScreen());
		road->addComponent(renderComponent);
		road->addComponent(renderReferenceComponent);
		road->addComponent(new RoadComponent(road3laneTextureBox.w/3, game->getScreen().w/2, 3));
	}
	road->update();
	return road;
}

Entity* SdlEntityFactory::createRoadBorder(Game* game, Renderer* renderer) {

	Rect roadBorderTextureBox;
	roadBorderTextureBox.w = game->getScreen().w/4;
	roadBorderTextureBox.h = roadBorderTextureBox.w;
	roadBorderTextureBox.x = roadBorderTextureBox.w/2;
	roadBorderTextureBox.y =  0;


	Entity * roadBorderLeft = NULL;
	if (checkrenderReferenceComponent()) {
		roadBorderLeft = game->createEntity();
		roadBorderLeft->addComponent(new PositionComponent(Point(game->getScreen().w *0.9, 0)));
		SdlRenderComponent * renderComponentL = new SdlRenderComponent(renderer);
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
		SdlRenderComponent * renderComponentR = new SdlRenderComponent(renderer);
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



