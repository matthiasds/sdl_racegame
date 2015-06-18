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

#define PATH_CAR1 "textures/Mercedes.png"
#define PATH_CAR2 "textures/Metallic_Red_Jeep_Liberty.png"
#define PATH_CAR3 "textures/Van.png"

#define PATH_ROAD_STRAIGHT_3_LANES "textures/Asphalt-road-complete.png"

#define PATH_GRASS_FULL "textures/grass.jpg"

#define PATH_RACEFONT "fonts/BREECBO_.TTF"
#define PATH_STANDARDFONT "fonts/ARIAL.TTF"

class SdlEntityFactory : public EntityFactory {
public:
	SdlEntityFactory();
	virtual ~SdlEntityFactory();
	Entity* createPlayerCar(Game* game, Renderer* renderer) override;
	Entity* createEnemyCar(Game* game, Renderer* renderer) override;
	Entity* createRoad(Game* game, Renderer* renderer) override;
	Entity* createRoadBorder(Game* game, Renderer* renderer) override;
	bool checkrenderReferenceComponent();
private:
	RenderReferenceComponent* renderReferenceComponent;
	Entity* road;
	Entity * playerCar;
};

#endif /* SDLENTITYFACTORY_H_ */
