/*

 * SdlInfoRenderSystem.h
 *
 *  Created on: 7-apr.-2015
 *      Author: Matthias
 */

#ifndef SDLINFORENDERSYSTEM_H_
#define SDLINFORENDERSYSTEM_H_

#include <bitset>
#include "BitSize.h"
#include "System.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "../components/VelocityComponent.h"
#include "../components/SdlInfoRenderComponent.h"
#include "../components/DamageComponent.h"

class SdlInfoRenderSystem : public System {
public:
	SdlInfoRenderSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~SdlInfoRenderSystem();
private:
	ComponentMapper<SdlInfoRenderComponent> sdlInfoRenderMapper;
	ComponentMapper<VelocityComponent> velocityMapper;
	ComponentMapper<DamageComponent> damageMapper;

	void drawSpeed(Entity* entity);
	void drawDamage(Entity* entity);
};


#endif /* SDLINFORENDERSYSTEM_H_ */
