/*
 * CarLaneMovingSystem.cpp
 *
 *  Created on: 8-apr.-2015
 *      Author: Matthias
 */

#include "CarLaneMovingSystem.h"
#include "ComponentTypeManager.h"
#include "../components/InputComponent.h"
#include "../components/LaneComponent.h"
#include "../components/PositionComponent.h"
#include "Entity.h"
#include "Game.h"




CarLaneMovingSystem::CarLaneMovingSystem() {
	addComponentType<LaneComponent>();
	addComponentType<PositionComponent>();
	addComponentType<VelocityComponent>();
	laneMovingState = LANE_FOLLOWING;
	currentRoadCenter = currentNumberOfLanes = currentLaneWidth = commingRoadCenter = commingNumberOfLanes = commingLaneWidth = 0;

}

void CarLaneMovingSystem::processEntity(Entity* entity) {

	updateLaneInfo(entity);
	int carWantedLane = LaneMapper.get(entity)->getWantedLane();

	if (carWantedLane > commingNumberOfLanes ) {
		carWantedLane = commingNumberOfLanes;
		laneMovingState = LANE_FOLLOWING;
	}
	else if (carWantedLane == -1) {
		laneMovingState = OUT_OF_LANE_FOLLOWING;
	}
	else if (carWantedLane < 1) {
		carWantedLane = 1;
		laneMovingState = LANE_FOLLOWING;
	}
	else {
		laneMovingState = LANE_FOLLOWING;
	}

	FPoint entityPosition = positionMapper.get(entity)->getPosition();

	int entityPositionX = (int) entityPosition.x;
	int closestLaneOffset;
	int closestLaneNumber;

	getClosestCurrentLaneAndOffset(entityPositionX, closestLaneOffset, closestLaneNumber);
	LaneMapper.get(entity)->setClosestLane(closestLaneNumber);
	LaneMapper.get(entity)->setClosestLaneOffset(closestLaneOffset);
	LaneMapper.get(entity)->setCurrentLane(closestLaneNumber);
	LaneMapper.get(entity)->setCurrentLaneWidth(currentLaneWidth);
	LaneMapper.get(entity)->setCommingNumberOfLanes(commingNumberOfLanes);

	VelocityComponent* thisVelocityComponent = velocityMapper.get(entity);
	std::bitset<BITSIZE> mySystemSpeedLock =  getSystemBit();

	switch (laneMovingState) {

		case LANE_FOLLOWING: {

			int velocityX = thisVelocityComponent->getVelocityX();
			int velocityY = std::abs(thisVelocityComponent->getVelocityY());



			int carWantedLanedifference = commingCenterLanePositions[carWantedLane-1] - entityPosition.x;
			if (carWantedLanedifference > 0 && thisVelocityComponent->testSpeedLockAvailable(X_PLUS_LOCK, mySystemSpeedLock)) { //car needs to go right and is allowed
				velocityX++; //increase right going speed
				thisVelocityComponent->setSpeedLock(X_PLUS_LOCK,mySystemSpeedLock);
				//but limit speed
				int limit = std::abs(velocityY)/2;
				velocityX = velocityX > limit ? limit : velocityX;
				if (entityPosition.x > commingCenterLanePositions[carWantedLane-1]) {
					entityPosition.x = commingCenterLanePositions[carWantedLane-1];
				}
			}
			else if (carWantedLanedifference < 0 && thisVelocityComponent->testSpeedLockAvailable(X_MIN_LOCK, mySystemSpeedLock)) {  //car needs to go left and is allowed
				velocityX--; //increase left going speed
				thisVelocityComponent->setSpeedLock(X_MIN_LOCK,mySystemSpeedLock);
				//but limit speed
				int limit = -std::abs(velocityY)/2;
				velocityX = velocityX < limit ? limit : velocityX;
				if (entityPosition.x < commingCenterLanePositions[carWantedLane-1]) {
					entityPosition.x = commingCenterLanePositions[carWantedLane-1];
				}
			}
			else {
				thisVelocityComponent->clrAllSpeedlocksOfSystem(mySystemSpeedLock); //release lock of speed;
				if (thisVelocityComponent->testSpeedLockAvailable()) {
					velocityX = 0;
				}
			}

			velocityMapper.get(entity)->setVelocityX(velocityX);
		}
		break;

		case OUT_OF_LANE_FOLLOWING: {
			//std::cout << "out of lane folowing on position: " << entityPosition.x << std::endl;
		}
		break;

	}

	//check grass driving left
	if (entityPosition.x < (currentCenterLanePositions[0] - currentLaneWidth/2.3) || entityPosition.x > (currentCenterLanePositions[currentNumberOfLanes - 1] + currentLaneWidth/2.3) ) {
		int newSpeed = (thisVelocityComponent->getVelocityY());
		if (newSpeed >=30  && thisVelocityComponent->testSpeedLockAvailable(X_MIN_LOCK, mySystemSpeedLock)) {
			newSpeed -= 2;
			thisVelocityComponent->clrAllSpeedlocksOfSystem(mySystemSpeedLock);
			thisVelocityComponent->setSpeedLock(X_MIN_LOCK,mySystemSpeedLock);
		} else if (newSpeed <=-20 && thisVelocityComponent->testSpeedLockAvailable(X_PLUS_LOCK, mySystemSpeedLock)) {
			thisVelocityComponent->clrAllSpeedlocksOfSystem(mySystemSpeedLock);
			thisVelocityComponent->setSpeedLock(X_PLUS_LOCK,mySystemSpeedLock);
			newSpeed += 2;
		}
		else {
			thisVelocityComponent->clrAllSpeedlocksOfSystem(mySystemSpeedLock);
		}
		//shake Car
		static int shakeVal;
		if (shakeVal !=4) {
			shakeVal = 4;
		}
		else {
			shakeVal = -4;
		}
		entityPosition.x += shakeVal;
		velocityMapper.get(entity)->setVelocityY(newSpeed);
	}
	else // out of grass
	{
		thisVelocityComponent->clrAllSpeedlocksOfSystem(mySystemSpeedLock);
	}



	positionMapper.get(entity)->setPosition(entityPosition);

}

void CarLaneMovingSystem::getClosestCurrentLaneAndOffset(int &entityPosition, int &offset, int &closestLaneNumber) {
	closestLaneNumber = 0;
	offset = std::numeric_limits<int>::max();
	for (int i = 0; i < currentNumberOfLanes; i++) {
		int laneEntityDifference = currentCenterLanePositions[i] - entityPosition;
		if ( std::abs(laneEntityDifference) <= std::abs(offset)) {
			offset = laneEntityDifference;
			closestLaneNumber = i + 1;
		}
	}
}

void CarLaneMovingSystem::getClosestCommingLaneAndOffset(int &entityPosition, int &offset, int &closestLaneNumber) {
	closestLaneNumber = 0;
	offset = std::numeric_limits<int>::max();
	for (int i = 0; i < commingNumberOfLanes; i++) {
		int laneEntityDifference = commingCenterLanePositions[i] - entityPosition;
		if ( std::abs(laneEntityDifference) <= std::abs(offset)) {
			offset = laneEntityDifference;
			closestLaneNumber = i + 1;
		}
	}
}


void CarLaneMovingSystem::updateLaneInfo(Entity* entity) {
	Entity* roadEntityReference = LaneMapper.get(entity)->getRoadEntityReference();
	currentRoadCenter = RoadMapper.get(roadEntityReference)->getCurrentRoadCenter();
	currentNumberOfLanes = RoadMapper.get(roadEntityReference)->getCurrentNumberOfLanes();
	currentLaneWidth = RoadMapper.get(roadEntityReference)->getCurrentLaneWidth();
	commingRoadCenter = RoadMapper.get(roadEntityReference)->getCommingRoadCenter();
	commingNumberOfLanes = RoadMapper.get(roadEntityReference)->getCommingNumberOfLanes();
	commingLaneWidth = RoadMapper.get(roadEntityReference)->getCommingLaneWidth();


	//calculate current lane center postions
	int outerLeftPosition = currentRoadCenter - currentNumberOfLanes*currentLaneWidth/2;
	currentCenterLanePositions.resize(currentNumberOfLanes);

	for (int i = 0; i < currentNumberOfLanes; i++) {
		currentCenterLanePositions[i] = outerLeftPosition + i*currentLaneWidth + currentLaneWidth/2;
	}
	outerLeftPosition = commingRoadCenter - commingNumberOfLanes*commingLaneWidth/2;
	commingCenterLanePositions.resize(commingNumberOfLanes);
	for (int i = 0; i < commingNumberOfLanes; i++) {
		commingCenterLanePositions[i] = outerLeftPosition + i*commingLaneWidth + currentLaneWidth/2;
	}
}





CarLaneMovingSystem::~CarLaneMovingSystem() {
	// TODO Auto-generated destructor stub
}



void CarLaneMovingSystem::init() {
}



