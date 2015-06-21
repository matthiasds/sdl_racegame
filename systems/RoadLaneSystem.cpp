/*
 * RoadLaneSystem.cpp
 *
 *  Created on: 13-apr.-2015
 *      Author: Matthias
 */

#include "RoadLaneSystem.h"
#include "Game.h"
#include "Sprite.h"

/**
 * RoadLaneSystem:
 * This is an intermediary system between the road tile generation of the BackgroundRenderComponent and the actual number of lanes
 * on the road. This number is set in the RoadComponent, used later by the CarLaneMovingSystem...
 *
 * Right now no road changes are done, but this makes it possible to make road blocks and extra lane interaction possible.
 */

/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */
RoadLaneSystem::RoadLaneSystem() {
	addComponentType<BackgroundTilingComponent>();
	addComponentType<RoadComponent>();
}


/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */
void RoadLaneSystem::processEntity(Entity* entity) {
	BackgroundTile* highestTile = backgroundTilingMapper.get(entity)->getHighestTile();
	BackgroundTile* referenceTile = backgroundTilingMapper.get(entity)->getReferenceTile();


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


/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
RoadLaneSystem::~RoadLaneSystem() {
}


/**
* init: not used in this System
*/
void RoadLaneSystem::init() {

}
