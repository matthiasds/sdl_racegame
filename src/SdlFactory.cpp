/*
 * SdlFactory.cpp
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#include <SdlFactory.h>

SdlFactory::SdlFactory() {
	// TODO Auto-generated constructor stub

}

SdlFactory::~SdlFactory() {
	// TODO Auto-generated destructor stub
}

Car* SdlFactory::createCar(RenderDevice* renderer) {
	return new SdlCar(renderer);
}

