/*
 * SdlMovingEntityDebugSystem.h
 *
 *  Created on: 10-may.-2015
 *      Author: Matthias
 */


#ifndef SDLMOVINGENTITYDEBUSYSTEM_H_
#define SDLMOVINGENTITYDEBUSYSTEM_H_

#include <bitset>
#include "BitSize.h"
#include "System.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "ComponentTypeManager.h"
#include "../components/VelocityComponent.h"
#include "../components/PositionComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/SdlDebugComponent.h"
#include "../components/RenderReferenceComponent.h"
#include "../components/DamageComponent.h"
#include "../components/LaneComponent.h"
#include "Game.h"


class SdlMovingEntityDebugSystem : public System {
public:
	SdlMovingEntityDebugSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~SdlMovingEntityDebugSystem();



private:
	ComponentMapper<VelocityComponent> velocityMapper;
	ComponentMapper<PositionComponent> positionMapper;
	ComponentMapper<CollisionComponent> collisionMapper;
	ComponentMapper<RenderReferenceComponent> renderReferenceMapper;
	ComponentMapper<SdlDebugComponent> debugMapper;
	ComponentMapper<DamageComponent> damageMapper;
	ComponentMapper<LaneComponent> laneMapper;

	void drawCollisionDebugInfo(Entity* entity);
	void drawDamageDebugInfo(Entity* entity);
	void drawSpeedDebugInfo(Entity* entity);
	void drawPositionDebugInfo(Entity* entity);
	void drawLaneDebugInfo(Entity* entity);
};

#endif /* SDLMOVINGENTITYDEBUSYSTEM_H_ */
