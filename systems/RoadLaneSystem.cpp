/*
 * BackgroundRenderSystem.cpp
 *
 *  Created on: 13-apr.-2015
 *      Author: Matthias
 */

#include "RoadLaneSystem.h"
#include "Game.h"
#include "Sprite.h"

RoadLaneSystem::RoadLaneSystem() {
	addComponentType<BackgroundRenderComponent>();
	addComponentType<RoadComponent>();
}

void RoadLaneSystem::processEntity(Entity* entity) {
	BackgroundTile* highestTile = backgroundRenderMapper.get(entity)->getHighestTile();
	BackgroundTile* referenceTile = backgroundRenderMapper.get(entity)->getReferenceTile();


	//comming road has 3 lanes?
	if (highestTile->getType() == ROAD_3_LANES) {
		roadMapper.get(entity)->setCommingNumberOfLanes(3);
		roadMapper.get(entity)->setCommingLaneWidth(game->getScreen().w/4.5);
		roadMapper.get(entity)->setCommingRoadCenter(game->getScreen().w/2);
	}
	//current road has 3 lanes?
	if (referenceTile->getType() == ROAD_3_LANES) {
		roadMapper.get(entity)->setCurrentNumberOfLanes(3);
		roadMapper.get(entity)->setCurrentLaneWidth(game->getScreen().w/4.5);
		roadMapper.get(entity)->setCurrentRoadCenter(game->getScreen().w/2);
	}
}

RoadLaneSystem::~RoadLaneSystem() {
	// TODO Auto-generated destructor stub
}

void RoadLaneSystem::init() {

}
