/*


 * DamageSystem.cpp
 *
 *  Created on: 11-may.-2015
 *      Author: Matthias
 */

#include "DamageSystem.h"
#include <cmath>

DamageSystem::DamageSystem() {
	addComponentType<DamageComponent>();
	addComponentType<CollisionComponent>();
}

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







DamageSystem::~DamageSystem() {
	// TODO Auto-generated destructor stub
}



void DamageSystem::init() {
}
