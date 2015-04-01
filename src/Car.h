/*
 * Car.h
 *
 *  Created on: 9-mrt.-2015
 *      Author: Matthias
 */

#ifndef CAR_H_
#define CAR_H_

#include "CommonIncludes.h"
#include "Entity.h"
#include "Utils.h"

class Car : public Entity {
private:
	int speed;

public:
	Car();
	//virtual void loadGraphics() =0;
	~Car();
	bool move();

	void set_direction();
};

#endif

