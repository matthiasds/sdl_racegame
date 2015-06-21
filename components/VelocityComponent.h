/*
 * VelocityComponent.h
 *
 *  Created on: 6-apr.-2015
 *      Author: Matthias
 */

#ifndef VELOCITYCOMPONENT_H_
#define VELOCITYCOMPONENT_H_

#include "BitSize.h"

/**
 * VelocityComponent:
 * This component keeps track of the containing entity's velocities in each direction.
 * Because many Systems need to change the velocity, problems with competing Systems arise
 * For this matter the velocityLock is introduced. A System can now lock increasing or decreasing the velocity in a certain direction.
 * In this way the locks work as a kind of MUTEX for competing systems velocity actions.
 *
 * Example of lock use:
 * For example when cars approach each other and want to prevent a collision the system responsible for preventing this can set a lock
 * on increasing the speed. by this an other system only allows the car to brake but not to increase speed. When the 2nd car brakes or limits
 * his speed. The System that detected the approaching collision can again release the lock
 */

enum DirectionLock {Y_MIN_LOCK = 0, Y_PLUS_LOCK, X_MIN_LOCK, X_PLUS_LOCK, NR_OF_DIRECTIONS};

class VelocityComponent : public IComponent {
public:

	/**
	 * Constructor:
	 * @param velocityX: initial velocity X
	 * @param velocityY initial velocity Y
	 */
	VelocityComponent(int velocityX, int velocityY) {
		this->velocityX = velocityX;
		this->velocityY = velocityY;
		originalSpeedY = 0;
	}

	/**
	 * gets the rotational velocity
	 * @return angularVelocity: rotational velocity of the components entity
	 */
	int getAngularVelocity() const {
		return angularVelocity;
	}

	/**
	 * Sets the rotational velocity
	 * @param angularVelocity: rotational velocity to be set
	 */
	void setAngularVelocity(int angularVelocity) {
		this->angularVelocity = angularVelocity;
	}

	/**
	 * gets velocity in X direction
	 * @return velocityX: velocity in X direction of the components entity
	 */
	int getVelocityX() {
		return velocityX;
	}

	/**
	 * Sets velocity in X direction
	 * @param velocityX: velocity in X direction to be set
	 */
	void setVelocityX(int velocityX) {
		this->velocityX = velocityX;
	}

	/**
	 * gets velocity in Y direction
	 * @return velocityY: velocity in Y direction of the components entity
	 */
	int getVelocityY(){
		return velocityY;
	}

	/**
	 * Sets velocity in Y direction
	 * @param velocityY: velocity in Y direction to be set
	 */
	void setVelocityY(int velocityY) {
		this->velocityY = velocityY;
	}

	/**
	 * Sets a SPECIFIC direction lock (X_MIN, X_PLUS, Y_MIN, Y_PLUS) for a certain System
	 * @param directionLock: The type of lock of the System that needs to be set = direction (X_MIN_LOCK, X_PLUS_LOCK, Y_MIN_LOCK, Y_PLUS_LOCK)
	 * @param velocityLock: The lock identity of of the System (bitset) to be set
	 */
	void setVelocityLock(DirectionLock directionLock , std::bitset<BITSIZE> velocityLock) {
		this->velocityLock[directionLock] = velocityLock;
	}

	/**
	 * Clears a SPECIFIC direction lock (X_MIN, X_PLUS, Y_MIN, Y_PLUS) a certain System has locked
	 * @param directionLock: The type of lock of the System that needs to be cleared = direction (X_MIN_LOCK, X_PLUS_LOCK, Y_MIN_LOCK, Y_PLUS_LOCK)
	 * @param velocityLock: The lock identity of of the System (bitset) to be cleared
	 */
	void clrVelocityLock(DirectionLock directionLock, std::bitset<BITSIZE> velocityLock) {
		this->velocityLock[directionLock] = 0;
	}

	/**
	 * Clears ALL the locks a certain System has locked
	 * @param velocityLockSystemToClear:  The lock identity of of the System (bitset) to be cleared
	 */
	void clrAllVelocityLocksOfSystem(std::bitset<BITSIZE> speedLockSystemToClear) {
		for (int i = 0; i < NR_OF_DIRECTIONS; i++ ) {
			if (velocityLock[i] == speedLockSystemToClear) {
				velocityLock[i] = 0;
			}
		}
	}

	/**
	 * Tests if no OTHER System has locked the request do do a CERTAIN speed action (X_MIN, X_PLUS, Y_MIN, Y_PLUS)
	 * @param directionLock: the type of speed action that is tested for (X_MIN_LOCK, X_PLUS_LOCK, Y_MIN_LOCK, Y_PLUS_LOCK)
	 * @param velocityLockToTest: The lock identity of of the requesting System (bitset) so this can be excluded
	 * @return true if the directionLock is free. So the requested velocity action is allowed
	 */
	bool testVelocityLockAvailable(DirectionLock directionLock, std::bitset<BITSIZE> velocityLockToTest) {
			if(velocityLock[directionLock] == 0 || velocityLock[directionLock] == velocityLockToTest) {
				return true;
			}
			return false;
	}

	/**
	 * Overload function of previous with no arguments
	 * Tests if no System (including the requesting system) has locked the request do do a ANY speed action (X_MIN, X_PLUS, Y_MIN, Y_PLUS)
	 * @return true if all directionLocks are free. So any  speed action is allowed
	 */
	bool testVelocityLockAvailable() {
		bool returnValue = true;
		for (int i = 0; i < NR_OF_DIRECTIONS; i++ ) {
			if (velocityLock[i] != 0) {
				returnValue = false;
			}
		}
		return returnValue;
	}

	/**
	 * getter for originalSpeedY: This is used to save the speed before a compensation action takes place to be able to restore it later
	 * @return originalSpeedY
	 */
	int getoriginalSpeedY() const {
			return originalSpeedY;
		}


	/**
	 * setter for originalSpeedY: This is used to save the speed before a compensation action takes place to be able to restore it later
	 * @param velocitylimitOffsetY
	 */
	void setoriginalSpeedY(int originalSpeedY) {
		this->originalSpeedY = originalSpeedY;
	}

	/**
	 * Destructor: No allocations to delete
	 */
	virtual ~VelocityComponent() {
	}



private:
	// velocity for left/right movement
	int velocityX;
	//velocity for forward backward movement
	int velocityY;
	//originalSpeed: place to save the speed before a compensation action takes place to be able to restore it later
	int originalSpeedY;
	// velocity for rotating movement
	int angularVelocity;
	// this array keeps for each direction (X_MIN, X_PLUS, Y_MIN, Y_PLUS) which system (represented by a bit in a bitset) has locked the velocity action in a certain direction
	std::bitset<BITSIZE> velocityLock[NR_OF_DIRECTIONS];
};

#endif /* VELOCITYCOMPONENT_H_ */
