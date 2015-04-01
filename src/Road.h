/*
 * Road.h
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#ifndef ROAD_H_
#define ROAD_H_

#include "Entity.h"

class Road : public Entity {
public:
	Road();
	virtual ~Road();
	virtual void advance() = 0;
};

#endif /* ROAD_H_ */
