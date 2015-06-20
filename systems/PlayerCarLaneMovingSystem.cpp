/*
 * PlayerCarLaneMovingSystem.cpp
 *
 *  Created on: 8-apr.-2015
 *      Author: Matthias
 */

#include "PlayerCarLaneMovingSystem.h"


/**
 * PlayerCarLaneMovingSystem:
 * This System is used for controlling the lane movement of the PlayerCar.
 * It uses the left, right inputs created by the (Sdl)InputSystem and creates wantedLane numbers
 * for the CarLaneMovingSystem. The playerCar can set the unique laneNumber -1 to indicate it is
 * positioning between lanes and not following any lane.
 *
 * In fact the player car has not much interaction with the lanes when controlled to keyboard.
 * This functions is implemented for later improvement for ex. for controlling by touch where it can
 * automatically center and folow a lane near the position of touch.
 * Other possible options would be like a temporary autopilot bonus...
 */


/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */
PlayerCarLaneMovingSystem::PlayerCarLaneMovingSystem() {
	addComponentType<LaneComponent>();
	addComponentType<PositionComponent>();
	addComponentType<VelocityComponent>();
	addComponentType<InputComponent>();
}


/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */
void PlayerCarLaneMovingSystem::processEntity(Entity* entity) {
	Inputs inputs = inputMapper.get(entity)->getInputs();

	if (inputs == LEFT || inputs == RIGHT) { //moving left or right?
		LaneMapper.get(entity)->setWantedLane(-1); //set out of lane movement
	}
	else { //stopped moving, check closest lane and snap to it
		int closestLane = LaneMapper.get(entity)->getClosestLane();
		int closestLaneOffset = LaneMapper.get(entity)->getClosestLaneOffset();
		int currentLaneWidth = LaneMapper.get(entity)->getCurrentLaneWidth();
		if (std::abs(closestLaneOffset) < currentLaneWidth * 0.3) {//if we came closer then 10% of lanewidth to lane center, snap to it
			LaneMapper.get(entity)->setWantedLane(closestLane);
		}
		else { //if not snapped to lane, set out of lane movement
			LaneMapper.get(entity)->setWantedLane(-1);
		}
	}
}


/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
PlayerCarLaneMovingSystem::~PlayerCarLaneMovingSystem() {

}

/**
* init: not used in this System
*/
void PlayerCarLaneMovingSystem::init() {
}



