/*
 * MovementSystem.h
 *
 *  Created on: 8-apr.-2015
 *      Author: Matthias
 */


#ifndef MOVEMENTSYSTEM_H_
#define MOVEMENTSYSTEM_H_

#include <bitset>
#include "BitSize.h"
#include "System.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "ComponentTypeManager.h"
#include "../components/VelocityComponent.h"
#include "../components/PositionComponent.h"
#include "../components/CollisionComponent.h"
#include "Game.h"

//this factor should be value between 0 and 1. 0 is minimum bump, 1 is maximum bump
#define BUMPFACTOR 0.2

class MovementSystem : public System {
public:
	MovementSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~MovementSystem();


private:
	ComponentMapper<VelocityComponent> velocityMapper;
	ComponentMapper<PositionComponent> positionMapper;
	ComponentMapper<CollisionComponent> collisionMapper;
};

#endif /* MOVEMENTSYSTEM_H_ */
