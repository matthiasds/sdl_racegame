/*
 * RoadPhysicsComponent.h
 *
 *  Created on: 2-apr.-2015
 *      Author: Matthias
 */

#ifndef ROADPHYSICSCOMPONENT_H_
#define ROADPHYSICSCOMPONENT_H_

#include "PhysicsComponent.h"

class Entity;
class Game;

class RoadPhysicsComponent : public PhysicsComponent {
public:
	RoadPhysicsComponent();
	void update(Entity& entity, Game& game) override;
	virtual ~RoadPhysicsComponent();
};

#endif /* ROADPHYSICSCOMPONENT_H_ */
