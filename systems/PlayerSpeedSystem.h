/*
 * PlayerSpeedSystem.h
 *
 *  Created on: 15-apr.-2015
 *      Author: Matthias
 */

#ifndef PLAYERSPEEDSYSTEM_H_
#define PLAYERSPEEDSYSTEM_H_

#include <bitset>
#include "BitSize.h"
#include "System.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "../components/InputComponent.h"
#include "../components/VelocityComponent.h"



class PlayerSpeedSystem : public System {
public:
	PlayerSpeedSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~PlayerSpeedSystem();
private:
	ComponentMapper<InputComponent> inputMapper;
	ComponentMapper<VelocityComponent> velocityMapper;
};



#endif /* PLAYERSPEEDSYSTEM_H_ */
