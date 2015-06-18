/*
 * PlayerCarLaneMovingSystem.h
 *
 *  Created on: 8-apr.-2015
 *      Author: Matthias
 */


#ifndef PLAYERCARLANEMOVINGSYSTEM_H_
#define PLAYERCARLANEMOVINGSYSTEM_H_

#include <bitset>
#include "BitSize.h"
#include "System.h"
#include "Entity.h"
#include "Game.h"
#include "ComponentMapper.h"
#include "../components/InputComponent.h"
#include "../components/LaneComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RoadComponent.h"


class PlayerCarLaneMovingSystem : public System {
public:
	PlayerCarLaneMovingSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~PlayerCarLaneMovingSystem();


private:
	ComponentMapper<LaneComponent> LaneMapper;
	ComponentMapper<PositionComponent> positionMapper;
	ComponentMapper<VelocityComponent> velocityMapper;
	ComponentMapper<InputComponent> inputMapper;
};



#endif /* PLAYERCARLANEMOVINGSYSTEM_H_ */
