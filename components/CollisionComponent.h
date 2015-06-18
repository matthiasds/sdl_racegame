/*
 * CollisionComponent.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Matthias
 */

#ifndef COLLISIONCOMPONENT_H_
#define COLLISIONCOMPONENT_H_

#include "IComponent.h"
#include "SharedCollisionComponent.h"
#include "Utils.h"

#define MAX_NR_OF_COLLISIONTYPES 4
enum CollisionTypeBit {BACK_COL, FRONT_COL, LEFT_COL, RIGHT_COL};
enum CollisionHandlingState {NO_COLLISION, NEW_COLLISION, INITIAL_COLLISION_HANDLING, AFTER_COLLISION_HANDLING, COLLISION_HANDLING_DONE};


class CollisionStatus {
public:
	CollisionHandlingState collisionHandlingState;
	Entity*	entity;
	std::bitset<MAX_NR_OF_COLLISIONTYPES> collisiontype;
	Point pointOfImpact;
	//values representing difference in speed between entity and colliding entity. positive = collision of other object to me, negative = collision of me to other object
	int speedOfImpactY;
	int speedOfImpactX;
};

class CollisionComponent : public IComponent {
public:
	CollisionComponent(Entity* entity, Rect collisionRect) {
		collisionEntity = entity;
		sharedCollisionComponent = &SharedCollisionComponent::getInstance();
		sharedCollisionComponent->addEntityToCollisionList(collisionEntity);
		this->collisionRect = collisionRect;
		collisionStatus.collisionHandlingState = NO_COLLISION;
		collisionStatus.entity = NULL;
		collisionStatus.collisiontype = 0;
		collisionStatus.pointOfImpact = Point(0,0);
		collisionStatus.speedOfImpactY = 0;
		collisionStatus.speedOfImpactX = 0;

	}
	~CollisionComponent() {
		sharedCollisionComponent->removeEntityFromCollisionList(collisionEntity);
	}
	std::vector<Entity*>& getCollisionList() {
			return sharedCollisionComponent->getCollisionList();
	}

	const Rect& getCollisionRect() const {
		return collisionRect;
	}

	void setCollisionRect(const Rect& collisionRect) {
		this->collisionRect = collisionRect;
	}

	void setCollisionStatus(CollisionHandlingState collisionHandlingState, Entity* entity, std::bitset<MAX_NR_OF_COLLISIONTYPES> collisiontype, Point pointOfImpact, int speedOfImpactX, int speedOfImpactY ){
		collisionStatus.collisionHandlingState = collisionHandlingState;
		collisionStatus.entity = entity;
		collisionStatus.collisiontype = collisiontype;
		collisionStatus.pointOfImpact = pointOfImpact;
		collisionStatus.speedOfImpactY = speedOfImpactY;
		collisionStatus.speedOfImpactX = speedOfImpactX;
	}

	void updatePointOfImpact(Point pointOfImpact ){
			collisionStatus.pointOfImpact = pointOfImpact;
	}

	void setCollisionStatusHandlingState(CollisionHandlingState collisionHandlingState){
		collisionStatus.collisionHandlingState = collisionHandlingState;
	}

	CollisionStatus& getCollisionStatus(){
		return collisionStatus;
	}

private:
	Entity * collisionEntity;
	SharedCollisionComponent* sharedCollisionComponent;
	Rect collisionRect;
	CollisionStatus collisionStatus;
};



extern SharedCollisionComponent sharedCollisionComponent;


#endif /* COLLISIONCOMPONENT_H_ */

