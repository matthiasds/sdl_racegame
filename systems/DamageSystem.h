/*
 * DamageSystem.h
 *
 *  Created on: 06-may.-2015
 *      Author: Matthias
 */

#ifndef DAMAGESYSTEM_H_
#define DAMAGESYSTEM_H_

#include <bitset>
#include "BitSize.h"
#include "System.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "../components/DamageComponent.h"
#include "../components/CollisionComponent.h"

class DamageSystem : public System {
public:
	DamageSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~DamageSystem();
private:
	ComponentMapper<DamageComponent> damageMapper;
	ComponentMapper<CollisionComponent> collisionMapper;
};


#endif /* DAMAGESYSTEM_H_ */
