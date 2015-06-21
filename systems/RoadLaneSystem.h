/*

 * RoadLaneSystem.h
 *
 *  Created on: 13-apr.-2015
 *      Author: Matthias
 */

#ifndef ROADLANESYSTEM_H_
#define ROADLANESYSTEM_H_

#include "System.h"
#include "BitSize.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "../components/BackgroundTilingComponent.h"
#include "../components/RoadComponent.h"

class Game;

class RoadLaneSystem : public System {
public:
	RoadLaneSystem();
	void init() override;
	virtual ~RoadLaneSystem();
	void processEntity(Entity* entity) override;


private:
	ComponentMapper<BackgroundTilingComponent> backgroundTilingMapper;
	ComponentMapper<RoadComponent> roadMapper;
};

#endif /* ROADLANESYSTEM_H_ */
