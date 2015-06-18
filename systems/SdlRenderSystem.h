/*
 * RenderSystem.h
 *
 *  Created on: 7-apr.-2015
 *      Author: Matthias
 */

#ifndef RENDERSYSTEM_H_
#define RENDERSYSTEM_H_

#include <bitset>
#include "BitSize.h"
#include "System.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "../components/RenderComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RenderReferenceComponent.h"

class SdlRenderSystem : public System {
public:
	SdlRenderSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~SdlRenderSystem();
private:
	ComponentMapper<RenderComponent> renderMapper;
	ComponentMapper<PositionComponent> positionMapper;
	ComponentMapper<RenderReferenceComponent> renderReferenceMapper;
};


#endif /* RENDERSYSTEM_H_ */
