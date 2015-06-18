/*
 * CollisionSystem.h
 *
 *  Created on: 7-apr.-2015
 *      Author: Matthias
 */

#ifndef COLLISIONSYSTEM_H_
#define COLLISIONSYSTEM_H_

#include <bitset>
#include "BitSize.h"
#include "System.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "../components/RenderComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RenderReferenceComponent.h"
#include "../components/CollisionComponent.h"

class CollisionSystem : public System {
public:
	CollisionSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~CollisionSystem();
private:
	ComponentMapper<RenderComponent> renderMapper;
	ComponentMapper<PositionComponent> positionMapper;
	ComponentMapper<RenderReferenceComponent> renderReferenceMapper;
	ComponentMapper<CollisionComponent> collisionMapper;
	ComponentMapper<VelocityComponent> velocityMapper;

	bool isEntityInRegionOfInterest(Entity* entity, int referencePosition);
	std::bitset<MAX_NR_OF_COLLISIONTYPES> analyzeCollisionType(Rect collisionBox, Point& pointOfImpact);
	bool checkCollision( Rect a, Rect b, Point& pointOfImpact );
};


#endif /* COLLISIONSYSTEM_H_ */
