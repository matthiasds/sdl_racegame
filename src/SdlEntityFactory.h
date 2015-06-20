/*
 * SdlEntityFactory.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef SDLENTITYFACTORY_H_
#define SDLENTITYFACTORY_H_

#include "EntityFactory.h"
#include "../components/RenderReferenceComponent.h"


class SdlEntityFactory : public EntityFactory {
public:
	SdlEntityFactory();
	virtual ~SdlEntityFactory();
	Entity* createPlayerCar(Game* game, Renderer* renderer) override;
	Entity* createEnemyCar(Game* game, Renderer* renderer, int position) override;
	Entity* createRoad(Game* game, Renderer* renderer) override;
	Entity* createRoadBorder(Game* game, Renderer* renderer) override;
	bool checkrenderReferenceComponent();
private:
	RenderReferenceComponent* renderReferenceComponent;
	Entity* road;
	Entity * playerCar;
	std::string getRandomCarTexture();
};

#endif /* SDLENTITYFACTORY_H_ */
