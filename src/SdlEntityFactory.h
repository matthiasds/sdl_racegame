/*
 * SdlEntityFactory.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef SDLENTITYFACTORY_H_
#define SDLENTITYFACTORY_H_

#include "EntityFactory.h"
#include "Renderer.h"
#include "SdlHardwareRenderer.h"

#include "../components/RenderReferenceComponent.h"

/** all systems used */
#include "../systems/MovementSystem.h"
#include "../systems/SdlRenderSystem.h"
#include "../systems/SdlInputSystem.h"
#include "../systems/PlayerSpeedSystem.h"
#include "../systems/AICarControlSystem.h"
#include "../systems/backgroundTilingSystem.h"
#include "../systems/roadLaneSystem.h"
#include "../systems/CarLaneMovingSystem.h"
#include "../systems/PlayerCarLaneMovingSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/SdlMovingEntityDebugSystem.h"
#include "../systems/SdlInfoRenderSystem.h"
#include "../systems/DamageSystem.h"
#include "../systems/AICarPlacementSystem.h"


class SdlEntityFactory : public EntityFactory {
public:
	SdlEntityFactory(Game* game);
	virtual ~SdlEntityFactory();
	Entity* createPlayerCar(Game* game) override;
	Entity* createEnemyCar(Game* game, int position) override;
	Entity* createRoad(Game* game) override;
	Entity* createRoadBorder(Game* game) override;
	void executeAllSystems() override;
	bool checkrenderReferenceComponent();
private:
	RenderReferenceComponent* renderReferenceComponent;
	Entity* road;
	Entity * playerCar;

	Renderer *renderDevice;

	SystemManager * systemManager;
	System * aICarControlSystem;
	System * sdlInputSystem;
	System * playerSpeedSystem;
	System * sdlInfoRenderSystem;
	System * movementSystem;
	System * renderSystem;
	System * backgroundTilingSystem;
	System * roadLaneSystem;
	System * carLaneMovingSystem;
	System * playerCarLaneMovingSystem;
	System * collisionSystem;
	System * sdlMovingEntityDebugSystem;
	System * damageSystem;
	System * aICarPlacementSystem;

	std::string getRandomCarTexture();
};

#endif /* SDLENTITYFACTORY_H_ */
