/*
 * EnemyCarControlSystem.cpp
 *
 *  Created on: 15-apr.-2015
 *      Author: Matthias
 */

#include "EnemyCarControlSystem.h"
#include <iostream>

EnemyCarControlSystem::EnemyCarControlSystem() {
	addComponentType<VelocityComponent>();
	addComponentType<AIComponent>();
	addComponentType<LaneComponent>();
	wantedLane = 1;
	lastLaneSwitch = std::chrono::system_clock::now();
	lastSpeedChange = std::chrono::system_clock::now();
}

void EnemyCarControlSystem::init() {

}

EnemyCarControlSystem::~EnemyCarControlSystem() {
	// TODO Auto-generated destructor stub
}


void EnemyCarControlSystem::processEntity(Entity* entity) {

	std::chrono::duration<double> secondsSindsLastLaneSwitch = std::chrono::system_clock::now() - lastLaneSwitch;
	std::chrono::duration<double> secondsSindsLastSpeedChange = std::chrono::system_clock::now() - lastSpeedChange;
	//std::cout << "elapsed time: " << secondsSindsLastLaneSwitch.count() << std::endl;
	std::chrono::duration<double> randomWaitTime {10};
	std::chrono::milliseconds speedIncreaseTimeout(50);


//	t1 = std::chrono::system_clock::now();
//
//	f(x); // do something
//
//	auto t2 = system_clock::now();
//	cout << "f(x) took " << duration_cast<milliseconds>(t2−t1).count() << " ms";


	int speed = velocityMapper.get(entity)->getVelocityY();

	int currentLane = LaneMapper.get(entity)->getCurrentLane();
	int commingNumberOfLanes = LaneMapper.get(entity)->getCommingNumberOfLanes();

	//check if we are at  wanted lane
	if(currentLane == wantedLane)
	{
		if (secondsSindsLastLaneSwitch > randomWaitTime) {
			//wait random time
			//choose random lane
			if (++wantedLane > commingNumberOfLanes) wantedLane = 1;
			lastLaneSwitch = std::chrono::system_clock::now();
		}
	}
	LaneMapper.get(entity)->setWantedLane(wantedLane);

	std::bitset<BITSIZE> mySystemSpeedLock =  getSystemBit();
	VelocityComponent* thisVelocityComponent = velocityMapper.get(entity);

	// limit speed increase
	if (secondsSindsLastSpeedChange > speedIncreaseTimeout) {
		if(speed < 30 && thisVelocityComponent->testSpeedLockAvailable(Y_PLUS_LOCK,mySystemSpeedLock)){
			speed++;
			thisVelocityComponent->setSpeedLock(Y_PLUS_LOCK,mySystemSpeedLock);
		} else if (speed > 30 && thisVelocityComponent->testSpeedLockAvailable(Y_MIN_LOCK,mySystemSpeedLock)) {
		   speed--;
		   thisVelocityComponent->setSpeedLock(Y_MIN_LOCK,mySystemSpeedLock);
		}
		else { //speed == 30
			thisVelocityComponent->clrAllSpeedlocksOfSystem(mySystemSpeedLock);
		}
		thisVelocityComponent->setVelocityY(speed);
		lastSpeedChange = std::chrono::system_clock::now();
	}
}




