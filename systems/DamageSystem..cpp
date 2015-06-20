/*


 * DamageSystem.cpp
 *
 *  Created on: 11-may.-2015
 *      Author: Matthias
 */

#include "DamageSystem.h"
#include <cmath>

/**
 * DamageSystem:
 * This System calculates a car's damage based on the CollisionComponent's saved parameter speedOfImpact.
 * Only Y direction impacts are calculated. So pushing a car at the side to make place does not result in damage.
 * The speed of the impact gives a proportional added damage percentage (100 = max). So light pushes to make a
 * preceding car going faster is allowed without taking damage.
 *
 * As the damage system only needs DamageComponent and CollisionComponent. This is applied to the PlayerCar as well as all
 * EnemyCars. Generally the EnemyCars will not take much damage as most of the time they will be hit in the back. Which does
 * not result in calculated damage. When the playerCars stops or drives in reverse an EnemyCar can hit the playerCar with his
 * front, so it is possible to have damage. But generally the PlayerCar should drive forward as this results in a higher score.
 */


/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */
DamageSystem::DamageSystem() {
	addComponentType<DamageComponent>();
	addComponentType<CollisionComponent>();
}

/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */
void DamageSystem::processEntity(Entity* entity) {
	CollisionStatus collisionStatus = collisionMapper.get(entity)->getCollisionStatus();
	//only add damage for front collisions not for side collisions
	if(collisionStatus.collisionHandlingState == COLLISION_HANDLING_DONE && collisionStatus.collisiontype.test(FRONT_COL) ) {
		int damage = damageMapper.get(entity)->getDamage();
		damage += std::abs(collisionMapper.get(entity)->getCollisionStatus().speedOfImpactY) * 0.1;
		if (damage > 100) { //limit damage
			damage = 100;
		}
		damageMapper.get(entity)->setDamage(damage);
	}
}


/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
DamageSystem::~DamageSystem() {
	// TODO Auto-generated destructor stub
}

/**
* init: not used in this System
*/
void DamageSystem::init() {
}
