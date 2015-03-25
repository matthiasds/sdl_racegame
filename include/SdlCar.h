/*
 * SdlCar.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef SDLCAR_H_
#define SDLCAR_H_

#include "RenderDevice.h"
#include "Car.h"


#define PATH_CAR1 "textures/car1.bmp"

class SdlCar : public Car {
private:
	RenderDevice *renderer;
public:
	SdlCar(RenderDevice *renderer);
	virtual ~SdlCar();
};

#endif /* SDLCAR_H_ */
