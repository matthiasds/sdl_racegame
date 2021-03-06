/*
 * EntityFactory.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef ENTITYFACTORY_H_
#define ENTITYFACTORY_H_

#include "Entity.h"

class EntityFactory {
public:
	virtual Entity* createPlayerCar(Game* game) = 0;
	virtual Entity* createEnemyCar(Game* game, int position) = 0;
	virtual Entity* createRoad(Game* game) = 0;
	virtual Entity* createRoadBorder(Game* game) = 0;
	virtual void executeAllSystems() = 0;
	virtual ~EntityFactory() {};
};

#endif /* ENTITYFACTORY_H_ */
