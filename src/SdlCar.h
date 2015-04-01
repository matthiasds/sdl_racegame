/*
 * SdlCar.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef SDLCAR_H_
#define SDLCAR_H_

#include "Renderer.h"
#include "Car.h"


#define PATH_CAR1 "textures/Gray_Mustang_GT.png"

class SdlCar : public Car {
public:
	SdlCar(Renderer *renderer);
	virtual ~SdlCar();
	Image* getImage() const override;
	Point	getOffset() const override;
	SubArea getSrc() const override;
	void addRenderables(std::vector<Renderable> &renderList) override;
private:
	Point offset;
	Image* carImage;
	SubArea image_source;
};


#endif /* SDLCAR_H_ */
