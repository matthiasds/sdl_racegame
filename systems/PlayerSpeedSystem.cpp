/*
 * PlayerSpeedSystem.cpp
 *
 *  Created on: 15-apr.-2015
 *      Author: Matthias
 */

#include "PlayerSpeedSystem.h"
#include <cmath>
#include <iostream>

PlayerSpeedSystem::PlayerSpeedSystem() {
	addComponentType<InputComponent>();
	addComponentType<VelocityComponent>();
}

void PlayerSpeedSystem::init() {
}


void PlayerSpeedSystem::processEntity(Entity* entity) {
	int velocityY = velocityMapper.get(entity)->getVelocityY();
	int velocityX = velocityMapper.get(entity)->getVelocityX();
	Inputs input = inputMapper.get(entity)->getInputs();
	std::bitset<BITSIZE> mySystemSpeedLock =  getSystemBit();
	VelocityComponent* thisVelocityComponent = velocityMapper.get(entity);

	if(input == UP){
		if (thisVelocityComponent->testSpeedLockAvailable(Y_PLUS_LOCK,mySystemSpeedLock)) {
			thisVelocityComponent->setSpeedLock(Y_PLUS_LOCK,mySystemSpeedLock);
			velocityY++;
		}
	}
	else if(input == DOWN){
		if (thisVelocityComponent->testSpeedLockAvailable(Y_MIN_LOCK,mySystemSpeedLock)) {
			thisVelocityComponent->setSpeedLock(Y_MIN_LOCK,mySystemSpeedLock);
			velocityY--;
		}
	}
	if(input == LEFT){
		if (thisVelocityComponent->testSpeedLockAvailable(X_MIN_LOCK,mySystemSpeedLock)) {
			thisVelocityComponent->setSpeedLock(X_MIN_LOCK,mySystemSpeedLock);
			velocityX--;
			//limit velocity
			int limit = - std::abs(velocityY)/2;
			velocityX = velocityX < limit ? limit : velocityX;
		}

	}
	else if(input == RIGHT){
		if (thisVelocityComponent->testSpeedLockAvailable(X_PLUS_LOCK,mySystemSpeedLock)) {
			thisVelocityComponent->setSpeedLock(X_PLUS_LOCK,mySystemSpeedLock);
			velocityX++;
			//limit velocity
			int limit = std::abs(velocityY)/2;
			velocityX = velocityX > limit ? limit : velocityX;
		}
	}
	else { //no key is pressed
		//go to 0 but not immediate but wit deceleration relative  to speed
		thisVelocityComponent->clrAllSpeedlocksOfSystem(mySystemSpeedLock); //free speed lock. Going to 0 can be overruled
		if (thisVelocityComponent->testSpeedLockAvailable()) {
			if (velocityX != 0) {
				velocityX -= velocityX/2;
			}
			if (std::abs(velocityX) <= 1) { //snap to 0
				velocityX = 0;
			}
		}
	}


	velocityMapper.get(entity)->setVelocityY(velocityY);
	velocityMapper.get(entity)->setVelocityX(velocityX);
}



PlayerSpeedSystem::~PlayerSpeedSystem() {
	// TODO Auto-generated destructor stub
}


