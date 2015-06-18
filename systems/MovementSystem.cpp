/*
 * MovementSystem.cpp
 *
 *  Created on: 8-apr.-2015
 *      Author: Matthias
 */

#include "MovementSystem.h"




MovementSystem::MovementSystem() {
	addComponentType<VelocityComponent>();
	addComponentType<PositionComponent>();
	addComponentType<CollisionComponent>();
}

void MovementSystem::processEntity(Entity* entity) {
	float gameDelta = game->getDelta();
	FPoint myPosition = positionMapper.get(entity)->getPosition();
	int velocityY = velocityMapper.get(entity)->getVelocityY();
	int velocityX = velocityMapper.get(entity)->getVelocityX();

	VelocityComponent* myVelocityComponent = velocityMapper.get(entity);
	std::bitset<BITSIZE> mySystemSpeedLock =  getSystemBit();

	CollisionStatus collisionstatus = collisionMapper.get(entity)->getCollisionStatus();
	switch (collisionstatus.collisionHandlingState) {

		/****************** HANDLE COLLISION MOVEMENT *****************/


		case NO_COLLISION: {
			myVelocityComponent->clrAllSpeedlocksOfSystem(mySystemSpeedLock);

		}
		break;
		case NEW_COLLISION: case INITIAL_COLLISION_HANDLING: { //this state indicates process but starts immediate (no break)

			/* ----- INITIAL COLLISION PHASE ---- */

			//exclusive FRONT-BACK collisions (excludes back + left...)
			if (collisionstatus.collisiontype == 1<<BACK_COL || collisionstatus.collisiontype == 1<<FRONT_COL) {  // (excludes back + left...)
				velocityMapper.get(entity)->setVelocityY(velocityY + collisionstatus.speedOfImpactY * (0.5 + BUMPFACTOR));  //make car bump relative to impact
				if (collisionstatus.collisiontype == 1<<BACK_COL) { // if a car collides in my back
					myVelocityComponent->setSpeedLock(Y_MIN_LOCK,mySystemSpeedLock); //Block going slower, do not test for other locks just overrule
				}
				else { // I collides in an enemy's back
					myVelocityComponent->setSpeedLock(Y_PLUS_LOCK,mySystemSpeedLock); //Block going faster, do not test for other locks just overrule
				}
			}

			//exclusive SIDE collisions
			if (collisionstatus.collisiontype == 1<< LEFT_COL || collisionstatus.collisiontype == 1<< RIGHT_COL) {  //if I hit/get hit on side (excludes back + left...)
				velocityMapper.get(entity)->setVelocityX(velocityX + collisionstatus.speedOfImpactX * (0.5 + BUMPFACTOR));  //make my car bump relative to velocity
				if (collisionstatus.collisiontype == 1<<LEFT_COL) { // if I get hit on the left side
					myVelocityComponent->setSpeedLock(X_MIN_LOCK,mySystemSpeedLock); //Block going left, do not test for other locks just overrule
				}
				else { // if I get hit on the right side
					myVelocityComponent->setSpeedLock(X_PLUS_LOCK, mySystemSpeedLock); //Block going right, do not test for other locks just overrule
				}
			}

			// CORNER collisions = LEFT + BACK ...
			if ((collisionstatus.collisiontype.test(LEFT_COL) || collisionstatus.collisiontype.test(RIGHT_COL)) && (collisionstatus.collisiontype.test(BACK_COL) || collisionstatus.collisiontype.test(FRONT_COL))) {  //if I hit/get hit on left back corner
				std::cout << "corner bump"<< std::endl;
				//do not act on front collision when other car is faster then me, otherwise car seems like being grapped when collide on pass
				if ((collisionstatus.collisiontype.test(FRONT_COL) && collisionstatus.speedOfImpactY < 0) || (collisionstatus.collisiontype.test(BACK_COL) && collisionstatus.speedOfImpactY > 0) ) velocityMapper.get(entity)->setVelocityY(velocityY + collisionstatus.speedOfImpactY * (0.6 + BUMPFACTOR));
				velocityMapper.get(entity)->setVelocityX(velocityX + collisionstatus.speedOfImpactX * (0.6 + BUMPFACTOR)); //make other car bump backward
				if (collisionstatus.collisiontype.test(FRONT_COL)) myVelocityComponent->setSpeedLock(Y_PLUS_LOCK,mySystemSpeedLock);
				if (collisionstatus.collisiontype.test(LEFT_COL)) myVelocityComponent->setSpeedLock(X_MIN_LOCK,mySystemSpeedLock);
				if (collisionstatus.collisiontype.test(RIGHT_COL)) myVelocityComponent->setSpeedLock(X_PLUS_LOCK,mySystemSpeedLock);
				if (collisionstatus.collisiontype.test(BACK_COL)) myVelocityComponent->setSpeedLock(Y_MIN_LOCK,mySystemSpeedLock);

			}

			//next state
			collisionMapper.get(entity)->setCollisionStatusHandlingState(AFTER_COLLISION_HANDLING);
		}
		break;

		case AFTER_COLLISION_HANDLING: {
			collisionMapper.get(entity)->setCollisionStatusHandlingState(COLLISION_HANDLING_DONE);
		}
		break;
		case COLLISION_HANDLING_DONE: { //if the CollisionSystem keeps this state active this means the collision is not solved so do it over
				collisionMapper.get(entity)->setCollisionStatusHandlingState(AFTER_COLLISION_HANDLING);
		}
		break;

	}

	/****************** HANDLE NORMAL/ NO-COLLISION MOVEMENT *****************/
	//Y movement
	myPosition.y += (float)gameDelta * velocityMapper.get(entity)->getVelocityY();
	//X movement
	myPosition.x += (float)gameDelta * velocityMapper.get(entity)->getVelocityX();

	positionMapper.get(entity)->setPosition(myPosition);
}

MovementSystem::~MovementSystem() {
	// TODO Auto-generated destructor stub
}




void MovementSystem::init() {

}
