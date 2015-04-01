/*
 * SdlFactory.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef SDLFACTORY_H_
#define SDLFACTORY_H_

#include "SdlCar.h"
#include "SdlRoad.h"
#include "EntityFactory.h"

class SdlFactory : public EntityFactory {
public:
	SdlFactory();
	virtual ~SdlFactory();
	virtual Car* createCar(Renderer* renderer) override;
	virtual Road* createRoad(Renderer* renderer) override;
};

#endif /* SDLFACTORY_H_ */
