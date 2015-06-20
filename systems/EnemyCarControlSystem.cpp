/*
 * EnemyCarControlSystem.cpp
 *
 *  Created on: 15-apr.-2015
 *      Author: Matthias
 */

#include "EnemyCarControlSystem.h"
#include "GameOptions.h"
#include <iostream>
#include <random>

/**
 * EnemyCarControlSystem:
 * This System controls the EnemyCars.
 * This systems mane task is lane switching for collision prevention. The system searches for neighboring EnemyCars and takes action based
 * upon the neighbors' positions. It uses a left take over. And no traffic keep right system as common in Europe. When there is no possibility
 * to take over the speed will be adapted to the preceding car.
 * Note that this systems only takes into account enemy  cars. It behaves like the PlayerCar is totally invisible so with the playerCar there
 * is no collision prevention. Otherwise the game would be too easy of course...
 * To switch lanes only the new wantedLane is set in the LaneComponent. The CarLaneMovingSystem takes care of the actual moving, following and
 * centering on the new lane.
 * The distance of which enemy cars become aware of a preceding, following or passing car is based on the laneWidth. If the factor is changed it
 * is possible cars start to late with taking over and collide anyway. Also in X direction the cars should only be awere of the cars 1 lane next
 * to them. If set to big. They will be aware of cars 2 lanes next to them and will not take over unless 2 lanes are free...
 *
 * As in Component EntitySystems the System is used for all entities containing certain components. An extra EnemyCarComponent is created to
 * make this exclusive.
 * Although it should possible to also use this system for the PlayerCar by the advantage of modularity of a Component Entity System.
 * For example a bonus for a 10s high speed autopilot for the PlayerCar could temporary add this System to the PlayerCar...
 *
 * As the processEntity function can only execute at 1 entity at a time there was a problem to analyze the interactions with the other entities
 * In most Component Entity Systems this is solved by other Systems and group managers that can behave on multiple entities. Here I have created
 * an own way of dealing with this deviating from the standard CES pattern. It works by creating an extra SharedEnemyCarComponent which is created as
 * singleton. Each entity creating the EnemyCarComponent is added to an entityList in the SharedEnemyCarComponent. Each EnemyCarComponent contains
 * a pointer to the sharedEnemyCarComponent so the entityList can be accessed from each component. In this way they entities subscribing to the same
 * component are aware of each other. In this way the component function is recycled in a creative way to prevent adding another complicated CES part
 * like the group manager.
 * Only future can tell if this deviation can work in all situations (new design patterns can only be proven to work by extensive use in different
 * applications). But in this rather limited game it works out quite well.
 */


/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */
EnemyCarControlSystem::EnemyCarControlSystem() {
	addComponentType<VelocityComponent>();
	addComponentType<PositionComponent>();
	addComponentType<AIComponent>();
	addComponentType<LaneComponent>();
	addComponentType<EnemyCarComponent>();
	lastLaneSwitch = std::chrono::system_clock::now();
	lastSpeedChange = std::chrono::system_clock::now();
}


/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */

void EnemyCarControlSystem::processEntity(Entity* entity) {

	int mySpeed = velocityMapper.get(entity)->getVelocityY();
	int currentLane = laneMapper.get(entity)->getCurrentLane();
	int commingNumberOfLanes = laneMapper.get(entity)->getCommingNumberOfLanes();
	int wantedLane = laneMapper.get(entity)->getWantedLane();

	Entity * neighboringEnemyCars[NR_OF_NEIGHBORCARPOSITIONS]; // gets filled  with neighboring entity's on all positions if in ROI
	std::bitset<NEIGHBOR_STATES> neighborStates;
	sortEnemyCarsInRegionOfInterest(entity, neighboringEnemyCars, neighborStates);

	/**** actions based on neighbor positions ****/

	if (neighborStates.test(NEIGHBORS)) //neighbors found in ROI?
	{
		for (int neighborPosition = 0; neighborPosition < NR_OF_NEIGHBORCARPOSITIONS; neighborPosition++ ) {
			Entity * neighbor = neighboringEnemyCars[neighborPosition];
			if (neighbor) {
				int neighborSpeed = velocityMapper.get(neighbor)->getVelocityY();
				switch (neighborPosition) {
				case NEIGHBOR_CAR_FRONT:  //car in front of me?
					if (mySpeed > neighborSpeed) { // I am faster, so I want to take over...
						//check if I am not on the left most lane  and nobody is on my side or left back (and not yet moving to this position)
						if (currentLane > 1 && !neighborStates.test(LEFT_NEIGHBORS)) {
							if (wantedLane == currentLane) { //prevent repeating this
								wantedLane--;
								laneMapper.get(entity)->setWantedLane(wantedLane);
							}

						}
						else { //other neighboring car preventing take over => limit speed
							mySpeed = neighborSpeed-1; //compensate for other system to increase
						}
					}
				}

			}
		}
	}
	else { //no neigbours found
		//Alone on the highway!... increase speed if wanted?
		//drive as right as possible = good trafic behaviour. All people shoud do that in real life too.
		if (currentLane < commingNumberOfLanes && currentLane == wantedLane) {
			wantedLane++;
			std::cout << "entity: " << entity->getId() << "wantedLane++ = " << wantedLane << std::endl;
			laneMapper.get(entity)->setWantedLane(wantedLane);
		}
	}

	if (!neighborStates.test(RIGHT_NEIGHBORS)) { //no right neighbors found in ROI?

	}

	std::chrono::duration<double> secondsSindsLastLaneSwitch = std::chrono::system_clock::now() - lastLaneSwitch;
	std::chrono::duration<double> secondsSindsLastSpeedChange = std::chrono::system_clock::now() - lastSpeedChange;
	//std::cout << "elapsed time: " << secondsSindsLastLaneSwitch.count() << std::endl;
	std::chrono::duration<double> randomWaitTime {10};
	std::chrono::milliseconds speedIncreaseTimeout(50);
	static std::default_random_engine generator;


//	t1 = std::chrono::system_clock::now();
//
//	f(x); // do something
//
//	auto t2 = system_clock::now();
//	cout << "f(x) took " << duration_cast<milliseconds>(t2−t1).count() << " ms";





	std::uniform_int_distribution<> random_lane(0,commingNumberOfLanes);

	//check if we are at  wanted lane
	/*if(currentLane == wantedLane)
	{
		if (secondsSindsLastLaneSwitch > randomWaitTime) {
			//wait random time
			//choose random lane
			wantedLane = random_lane(generator);
			lastLaneSwitch = std::chrono::system_clock::now();
		}
	}*/


	std::bitset<BITSIZE> mySystemSpeedLock =  getSystemBit();
	VelocityComponent* thisVelocityComponent = velocityMapper.get(entity);

	// limit mySpeed increase
	if (secondsSindsLastSpeedChange > speedIncreaseTimeout) {
		if(mySpeed < 30 && thisVelocityComponent->testSpeedLockAvailable(Y_PLUS_LOCK,mySystemSpeedLock)){
			mySpeed++;
			thisVelocityComponent->setSpeedLock(Y_PLUS_LOCK,mySystemSpeedLock);
		} else if (mySpeed > 30 && thisVelocityComponent->testSpeedLockAvailable(Y_MIN_LOCK,mySystemSpeedLock)) {
			mySpeed--;
		   thisVelocityComponent->setSpeedLock(Y_MIN_LOCK,mySystemSpeedLock);
		}
		else { //speed == 30
			thisVelocityComponent->clrAllSpeedlocksOfSystem(mySystemSpeedLock);
		}
		thisVelocityComponent->setVelocityY(mySpeed);
		lastSpeedChange = std::chrono::system_clock::now();
	}
}


/**
 * This function analyzes all enemy cars. Determines if they are in ROI and determines the type of neighboring (bitset). An array with the reference to each neighbor
 * is filled (if there is one).
 * @param entity: current entity which is used as container for the position and as container for the EnemyCarList containing the interacting entities ->argument
 * @param neighboringEnemyCars: array which is filled with the pointers to the neighboring cars' entities. -> return
 * @param neighborStates: bitset which is filled with the (possible multiple) positions of neighbors -> return
 */
void EnemyCarControlSystem::sortEnemyCarsInRegionOfInterest(Entity* entity, Entity ** neighboringEnemyCars, std::bitset<NEIGHBOR_STATES> & neighborStates) {

	int takeOverRangeY= laneMapper.get(entity)->getCurrentLaneWidth()*1.5;
	int takeOverRangeX= laneMapper.get(entity)->getCurrentLaneWidth()/2;

	std::vector<Entity*> enemyCarList = enemyCarMapper.get(entity)->getEnemyCarList();
	bool front, back, nextTo, neighbors;

	//get my position
	FPoint myEnityPosition = positionMapper.get(entity)->getPosition();
	//first set all neighbor car pointers in array to NULL
	for (int neighborPosition = 0; neighborPosition < NR_OF_NEIGHBORCARPOSITIONS; neighborPosition++ ) {
		neighboringEnemyCars[neighborPosition] = NULL;
	}

	// now find neighbors in all cars in the region but not myself
	for (auto const &currentEnemyCar : enemyCarList) {
		neighbors = true;
		front = back = nextTo = false;
		if (currentEnemyCar != entity) { //check the other car is not myself
			FPoint possibleNeighbourPosition = positionMapper.get(currentEnemyCar)->getPosition();

			//possible neighbor car in front of my car in ROI?
			if (possibleNeighbourPosition.y > (myEnityPosition.y + takeOverRangeY) && possibleNeighbourPosition.y <  (myEnityPosition.y +2*takeOverRangeY)) {
				front = true;
				neighborStates.set(FRONT_NEIGHBORS);
			}
			//possible neighbor car behind my car in ROI?
			else if (possibleNeighbourPosition.y < (myEnityPosition.y - takeOverRangeY) && possibleNeighbourPosition.y >  (myEnityPosition.y - 2*takeOverRangeY)) {
				back = true;
				neighborStates.set(BACK_NEIGHBORS);
			}
			//possible neighbor car on same height as me (=next to me)?
			else if (possibleNeighbourPosition.y < (myEnityPosition.y + takeOverRangeY) && possibleNeighbourPosition.y >  (myEnityPosition.y - takeOverRangeY)) {
				nextTo = true;
				neighborStates.set(SIDE_NEIGHBORS);
			}
			// no neighbor in ROI (Y_position)
			else {
				neighbors = false;
			}

			if (neighbors) { // if any car in Y_direction ROI define left-right position
				neighborStates.set(NEIGHBORS);
				//possible neighbor car right of my car in ROI?
				if (possibleNeighbourPosition.x > (myEnityPosition.x + takeOverRangeX) && possibleNeighbourPosition.x <  (myEnityPosition.x + 2*takeOverRangeX)) {
					neighborStates.set(RIGHT_NEIGHBORS);
					if (front) {
						neighboringEnemyCars[NEIGHBOR_CAR_FRONT_RIGHT] = currentEnemyCar;
					} else if (back) {
						neighboringEnemyCars[NEIGHBOR_CAR_BACK_RIGHT] = currentEnemyCar;
					} else {
						neighboringEnemyCars[NEIGHBOR_CAR_RIGHT] = currentEnemyCar;
					}
				}
				//possible neighbor car left of my car in ROI?
				else if (possibleNeighbourPosition.x < (myEnityPosition.x - takeOverRangeX) && possibleNeighbourPosition.x <  (myEnityPosition.x - 2*takeOverRangeX)) {
					neighborStates.set(LEFT_NEIGHBORS);
					if (front) {
						neighboringEnemyCars[NEIGHBOR_CAR_FRONT_LEFT] = currentEnemyCar;
					} else if (back) {
						neighboringEnemyCars[NEIGHBOR_CAR_BACK_LEFT] = currentEnemyCar;
					} else {
						neighboringEnemyCars[NEIGHBOR_CAR_LEFT] = currentEnemyCar;
					}
				}
				else if (front) {
					neighboringEnemyCars[NEIGHBOR_CAR_FRONT] = currentEnemyCar;
				}
				else if (back) {
					neighboringEnemyCars[NEIGHBOR_CAR_BACK] = currentEnemyCar;
				}
			}
		}
	}
}


/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
EnemyCarControlSystem::~EnemyCarControlSystem() {
	// TODO Auto-generated destructor stub
}

/**
* init: not used in this System
*/
void EnemyCarControlSystem::init() {

}










