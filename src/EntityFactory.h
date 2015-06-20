/*
 * EntityFactory.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef ENTITYFACTORY_H_
#define ENTITYFACTORY_H_

#include "Car.h"
#include "Road.h"

class EntityFactory {
public:
	virtual Entity* createPlayerCar(Game* game, Renderer* renderer) = 0;
	virtual Entity* createEnemyCar(Game* game, Renderer* renderer, int position) = 0;
	virtual Entity* createRoad(Game* game, Renderer* renderer) = 0;
	virtual Entity* createRoadBorder(Game* game, Renderer* renderer) = 0;
	virtual ~EntityFactory() {};
};

#endif /* ENTITYFACTORY_H_ */
