/*
 * CarPhysicsComponent.h
 *
 *  Created on: 2-apr.-2015
 *      Author: Matthias
 */

#ifndef CARPHYSICSCOMPONENT_H_
#define CARPHYSICSCOMPONENT_H_

#include "PhysicsComponent.h"

class Entity;
class Game;

class CarPhysicsComponent : public PhysicsComponent {
public:
	CarPhysicsComponent();
	void update(Entity& entity, Game& game) override;
	virtual ~CarPhysicsComponent();
};

#endif /* CARPHYSICSCOMPONENT_H_ */
