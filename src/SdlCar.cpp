/*
 * SdlCar.cpp
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#include <SdlCar.h>
#include "Car.h"

SdlCar::SdlCar(RenderDevice *renderer) : Car(renderer) {
	this->renderer=renderer;
	entityRender->image=renderer->loadImage(PATH_CAR1);
}


SdlCar::~SdlCar() {
	// TODO Auto-generated destructor stub
}

