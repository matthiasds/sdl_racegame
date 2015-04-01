/*
 * SdlRoad.h
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#ifndef SDLROAD_H_
#define SDLROAD_H_

#include "Renderer.h"
#include "Road.h"

#define PATH_ROAD_STRAIGHT_3_LANES "textures/Asphalt-road-complete.png"

class SdlRoad : public Road {
public:
	SdlRoad(Renderer *renderer);
	virtual ~SdlRoad();
	Image* getImage() const override;
	Point	getOffset() const override;
	SubArea getSrc() const override;
	void addRenderables(std::vector<Renderable> &renderList) override;
	void advance() override;
private:
	Point offset;
	std::vector<Image*> roadImages;
	SubArea spriteCutOut;
};
#endif /* SDLROAD_H_ */
