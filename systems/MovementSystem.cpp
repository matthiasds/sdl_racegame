/*
 * MovementSystem.cpp
 *
 *  Created on: 8-apr.-2015
 *      Author: Matthias
 */

#include "MovementSystem.h"

/**
 * MovementSystem:
 * This System takes care of the vertical and horizontal displacement of car (and eventually other entities with a speed and which are collidable)
 * Velocity multiplied by a gameDelta is used as the base of moving entities.
 * Mind that in this System the reference (usually player car) is not considered. This is only done during rendering as it is just a camera viewpoint.
 * The actual coordinates keep increasing. One should consider to end the level before the Y coordinate overflows or implement a wrap over function.
 * Right now this is not implemented yet.
 *
 * The MovementSystem also handles the collisions detected by the collisionDetection System. It does this by displacing the entities based on the
 * speedOfImpact emulating a proportional bounce effect.
 * Mind this system does this entity per entity, so without being aware of other entities only by the saved states of the collision saved by the CollisionSystem
 * (The CollisionSystem is aware of multiple entities). Interactions between entities should be reduced as it is very cpu consuming by the many loops needed to check every entity with one other.
 *
 */

/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */

MovementSystem::MovementSystem() {
	addComponentType<VelocityComponent>();
	addComponentType<PositionComponent>();
	addComponentType<CollisionComponent>();
}


/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */
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
			myVelocityComponent->clrAllVelocityLocksOfSystem(mySystemSpeedLock);

		}
		break;
		case NEW_COLLISION: case INITIAL_COLLISION_HANDLING: { //this state indicates process but starts immediate (no break)

			/* ----- INITIAL COLLISION PHASE ---- */

			//exclusive FRONT-BACK collisions (excludes back + left...)
			if (collisionstatus.collisiontype == 1<<BACK_COL || collisionstatus.collisiontype == 1<<FRONT_COL) {  // (excludes back + left...)
				velocityMapper.get(entity)->setVelocityY(velocityY + collisionstatus.speedOfImpactY * (0.5 + BUMPFACTOR));  //make car bump relative to impact
				if (collisionstatus.collisiontype == 1<<BACK_COL) { // if a car collides in my back
					myVelocityComponent->setVelocityLock(Y_MIN_LOCK,mySystemSpeedLock); //Block going slower, do not test for other locks just overrule
				}
				else { // I collides in an enemy's back
					myVelocityComponent->setVelocityLock(Y_PLUS_LOCK,mySystemSpeedLock); //Block going faster, do not test for other locks just overrule
				}
			}

			//exclusive SIDE collisions
			if (collisionstatus.collisiontype == 1<< LEFT_COL || collisionstatus.collisiontype == 1<< RIGHT_COL) {  //if I hit/get hit on side (excludes back + left...)
				velocityMapper.get(entity)->setVelocityX(velocityX + collisionstatus.speedOfImpactX * (0.5 + BUMPFACTOR));  //make my car bump relative to velocity
				if (collisionstatus.collisiontype == 1<<LEFT_COL) { // if I get hit on the left side
					myVelocityComponent->setVelocityLock(X_MIN_LOCK,mySystemSpeedLock); //Block going left, do not test for other locks just overrule
				}
				else { // if I get hit on the right side
					myVelocityComponent->setVelocityLock(X_PLUS_LOCK, mySystemSpeedLock); //Block going right, do not test for other locks just overrule
				}
			}

			// CORNER collisions = LEFT + BACK ...
			if ((collisionstatus.collisiontype.test(LEFT_COL) || collisionstatus.collisiontype.test(RIGHT_COL)) && (collisionstatus.collisiontype.test(BACK_COL) || collisionstatus.collisiontype.test(FRONT_COL))) {  //if I hit/get hit on left back corner
				std::cout << "corner bump"<< std::endl;
				//do not act on front collision when other car is faster then me, otherwise car seems like being grabbed when collide on pass
				if ((collisionstatus.collisiontype.test(FRONT_COL) && collisionstatus.speedOfImpactY < 0) || (collisionstatus.collisiontype.test(BACK_COL) && collisionstatus.speedOfImpactY > 0) ) velocityMapper.get(entity)->setVelocityY(velocityY + collisionstatus.speedOfImpactY * (0.6 + BUMPFACTOR));
				velocityMapper.get(entity)->setVelocityX(velocityX + collisionstatus.speedOfImpactX * (0.6 + BUMPFACTOR)); //make other car bump backward
				//deadlock? if hit on both corners...
				if (collisionstatus.collisiontype.test(BACK_COL) && collisionstatus.collisiontype.test(FRONT_COL) && collisionstatus.collisiontype.test(RIGHT_COL) && collisionstatus.collisiontype.test(LEFT_COL)) {
					//only lock front and right so left and right is a solution
					myVelocityComponent->setVelocityLock(Y_PLUS_LOCK,mySystemSpeedLock);
					myVelocityComponent->setVelocityLock(Y_MIN_LOCK,mySystemSpeedLock);
				}
				else { //no deadlock:
					if (collisionstatus.collisiontype.test(FRONT_COL)) myVelocityComponent->setVelocityLock(Y_PLUS_LOCK,mySystemSpeedLock);
					if (collisionstatus.collisiontype.test(LEFT_COL)) myVelocityComponent->setVelocityLock(X_MIN_LOCK,mySystemSpeedLock);
					if (collisionstatus.collisiontype.test(RIGHT_COL)) myVelocityComponent->setVelocityLock(X_PLUS_LOCK,mySystemSpeedLock);
					if (collisionstatus.collisiontype.test(BACK_COL)) myVelocityComponent->setVelocityLock(Y_MIN_LOCK,mySystemSpeedLock);
				}
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

/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
MovementSystem::~MovementSystem() {
}


/**
* init: not used in this System
*/
void MovementSystem::init() {

}
