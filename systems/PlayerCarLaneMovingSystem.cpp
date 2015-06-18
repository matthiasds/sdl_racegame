/*
 * PlayerCarLaneMovingSystem.cpp
 *
 *  Created on: 8-apr.-2015
 *      Author: Matthias
 */

#include "PlayerCarLaneMovingSystem.h"





PlayerCarLaneMovingSystem::PlayerCarLaneMovingSystem() {
	addComponentType<LaneComponent>();
	addComponentType<PositionComponent>();
	addComponentType<VelocityComponent>();
	addComponentType<InputComponent>();
}

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


PlayerCarLaneMovingSystem::~PlayerCarLaneMovingSystem() {
	// TODO Auto-generated destructor stub
}



void PlayerCarLaneMovingSystem::init() {
}



