/*
 * EntityFactory.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef ENTITYFACTORY_H_
#define ENTITYFACTORY_H_

#include "Car.h"

class EntityFactory {
public:
	virtual Car* createCar(RenderDevice* renderer) = 0;
	virtual ~EntityFactory();
};

#endif /* ENTITYFACTORY_H_ */
