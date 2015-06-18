/*
 * VelocityComponent.h
 *
 *  Created on: 6-apr.-2015
 *      Author: Matthias
 */

#ifndef VELOCITYCOMPONENT_H_
#define VELOCITYCOMPONENT_H_

#include "BitSize.h"

#define NR_OF_DIRECTIONS 4

enum DirectionLock {Y_MIN_LOCK = 0, Y_PLUS_LOCK, X_MIN_LOCK, X_PLUS_LOCK};

class VelocityComponent : public IComponent {
public:


	VelocityComponent(int velocityX, int velocityY) {
		this->velocityX = velocityX;
		this->velocityY = velocityY;
	}

	int getAngularVelocity() const {
		return angularVelocity;
	}

	void setAngularVelocity(int angularVelocity) {
		this->angularVelocity = angularVelocity;
	}

	int getVelocityX() {
		return velocityX;
	}

	void setVelocityX(int velocityX) {
		this->velocityX = velocityX;
	}

	int getVelocityY(){
		return velocityY;
	}

	void setVelocityY(int velocityY) {
		this->velocityY = velocityY;
	}


	virtual ~VelocityComponent() {
	}


	void setSpeedLock(DirectionLock directionLock , std::bitset<BITSIZE> speedLock) {
		this->speedLock[directionLock] = speedLock;
	}

	void clrSpeedLock(DirectionLock directionLock, std::bitset<BITSIZE> speedLock) {
		this->speedLock[directionLock] = 0;
	}

	void clrAllSpeedlocksOfSystem(std::bitset<BITSIZE> speedLockSystemToClear) {
		for (int i = 0; i < NR_OF_DIRECTIONS; i++ ) {
			if (speedLock[i] == speedLockSystemToClear) {
				speedLock[i] = 0;
			}
		}
	}

	bool testSpeedLockAvailable(DirectionLock directionLock, std::bitset<BITSIZE> speedLockToTest) {
			if(speedLock[directionLock] == 0 || speedLock[directionLock] == speedLockToTest) {
				return true;
			}
			return false;
	}

	//overload with no argument to check for any speedLock
	bool testSpeedLockAvailable() {
		bool returnValue = true;
		for (int i = 0; i < NR_OF_DIRECTIONS; i++ ) {
			if (speedLock[i] != 0) {
				returnValue = false;
			}
		}
		return returnValue;
	}

private:
	int velocityX;
	int velocityY;
	int angularVelocity;
	std::bitset<BITSIZE> speedLock[NR_OF_DIRECTIONS];
};

#endif /* VELOCITYCOMPONENT_H_ */
