/*
 * SdlCar.cpp
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#include "SdlCar.h"
#include "Car.h"

SdlCar::SdlCar(Renderer *renderer) : carImage(NULL)
 {
	setRenderer(renderer);
	carImage=getRenderer()->loadImage(PATH_CAR1);
	SDL_Rect car_image_area;
	car_image_area.h=640;
	car_image_area.w=280;
	car_image_area.x=100;
	car_image_area.y=0;
	image_source=car_image_area;
	offset=Point(0,0);
	pos=FPoint(Point(100,100));
	carImage->setDestinationScaleFactor(1.7);
}


SdlCar::~SdlCar() {
	// TODO call freeimage
}

Image* SdlCar::getImage() const {
	return carImage;
}


Point SdlCar::getOffset() const {
	return offset;
}



SubArea SdlCar::getSrc() const {
	return image_source;
}

void SdlCar::addRenderables(std::vector<Renderable>& renderList) {
}
