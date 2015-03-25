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
	Car(RenderDevice* renderer);
	//virtual void loadGraphics() =0;
	~Car();
	bool move();
	void addRenders(std::vector<Renderable> &r);
	void set_direction();
};

#endif

