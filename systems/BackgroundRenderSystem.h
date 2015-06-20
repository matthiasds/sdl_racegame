/*

 * BackgroundRenderSystem.h
 *
 *  Created on: 13-apr.-2015
 *      Author: Matthias
 */

#ifndef BACKGROUNDRENDERSYSTEM_H_
#define BACKGROUNDRENDERSYSTEM_H_

#include "System.h"
#include "BitSize.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "../components/BackgroundRenderComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RenderReferenceComponent.h"
#include "../components/SdlRenderComponent.h"

class Game;

class BackgroundRenderSystem : public System {
public:
	BackgroundRenderSystem();
	void init() override;
	virtual ~BackgroundRenderSystem();
	void processEntity(Entity* entity) override;


private:
	ComponentMapper<BackgroundRenderComponent> backgroundRenderMapper;
	ComponentMapper<PositionComponent> positionMapper;
	ComponentMapper<RenderReferenceComponent> renderReferenceMapper;
	ComponentMapper<SdlRenderComponent> renderMapper;
};

#endif /* BACKGROUNDRENDERSYSTEM_H_ */
