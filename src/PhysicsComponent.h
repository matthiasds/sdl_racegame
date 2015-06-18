/*
 * PhysicsComponent.h
 *
 *  Created on: 1-apr.-2015
 *      Author: Matthias
 */

#ifndef PHYSICSCOMPONENT_H_
#define PHYSICSCOMPONENT_H_

class Game;
class Entity;

class PhysicsComponent {
public:
	virtual ~PhysicsComponent() {};
	virtual void update(Entity& entity, Game& game) = 0;
};

#endif /* PHYSICSCOMPONENT_H_ */
