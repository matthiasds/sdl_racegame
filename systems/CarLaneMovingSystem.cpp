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

/**
 * CarLaneMovingSystem:
 * This system  takes care of the lane switching as well for player cars as for other (enemy) cars.
 * For enemy cars tracking the lane center position is important to prevent chaos on road changes.
 * Also for player cars there is a kind of automatic correction when coming close to a lane center it will snap
 * the car neatly in the center. When using touch instead of keyboard controls this center snapping could be increased
 * so when you touch a lane it goes neatly to center...
 *
 * There is functionality for change of lane nr's. This could be reduced for obstacles on a lane or increased when the
 * road becomes wider (more lanes) on a certain point.
 * The player car is the only one who uses the lane -1. This is an indication of inter Lane driving, in this way it can drive
 * between lanes without belonging to a certain lain. Ones coming close enough to a lane center this lane number is applied.
 * Wanted and current lane numbers are especially useful for enemy cars who use the system to follow the road and taking over
 * when they drive faster then the car before them. This latest functionality is done in the EnemyCarControlSystem.
 *
 * If the car goes beyond the border of the road it is detected by this system and some car shaking and speed limit
 * is applied to indicate driving in the grass.
 */


/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */
CarLaneMovingSystem::CarLaneMovingSystem() {
	addComponentType<LaneComponent>();
	addComponentType<PositionComponent>();
	addComponentType<VelocityComponent>();
	laneMovingState = LANE_FOLLOWING;
	currentRoadCenter = currentNumberOfLanes = currentLaneWidth = commingRoadCenter = commingNumberOfLanes = commingLaneWidth = 0;

}


/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */
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
				velocityX+=2; //increase right going speed
				thisVelocityComponent->setSpeedLock(X_PLUS_LOCK,mySystemSpeedLock);
				//but limit speed
				int limit = std::abs(velocityY)/2;
				velocityX = velocityX > limit ? limit : velocityX;
				//prevent going over center and start oscillation
				if (entityPosition.x > commingCenterLanePositions[carWantedLane-1]-1) {
					entityPosition.x = commingCenterLanePositions[carWantedLane-1];
				}
			}
			else if (carWantedLanedifference < 0 && thisVelocityComponent->testSpeedLockAvailable(X_MIN_LOCK, mySystemSpeedLock)) {  //car needs to go left and is allowed
				velocityX-=2; //increase left going speed
				thisVelocityComponent->setSpeedLock(X_MIN_LOCK,mySystemSpeedLock);
				//but limit speed
				int limit = -std::abs(velocityY)/2;
				velocityX = velocityX < limit ? limit : velocityX;
				//prevent going over center and start oscillation
				if (entityPosition.x < commingCenterLanePositions[carWantedLane-1]+1) {
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
	if (entityPosition.x < (currentCenterLanePositions[0] - currentLaneWidth/2.4) || entityPosition.x > (currentCenterLanePositions[currentNumberOfLanes - 1] + currentLaneWidth/2.4) ) {
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

/**
 * Private function to determine the CURRENT closest lane number based on the current X position of the entity
 * @param entityPosition: current X position of the entity -> argument
 * @param offset: at return this is filled with the distance to the lane center -> return
 * @param closestLaneNumber: at return this is filled with the closest lane number -> return
 */
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

/**
 * Private function to determine the COMMING closest lane number based on the current X position of the entity
 * This can be used to anticipate for road changes as reducing or increasing number of lanes.
 * @param entityPosition: current X position of the entity -> argument
 * @param offset: at return this is filled with the distance to the lane center -> return
 * @param closestLaneNumber: at return this is filled with the closest lane number -> return
 */
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

/**
 * This function retrieves all the used parameters at the entity's components and caches them in
 * temporary variables used in multiple places during the processing.
 * @param entity: entity is always needed as argument as it is the container for all components
 */
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




/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
CarLaneMovingSystem::~CarLaneMovingSystem() {
	// TODO Auto-generated destructor stub
}


/**
* init: not used in this System
*/
void CarLaneMovingSystem::init() {
}



