/*
 * SdlRoad.cpp
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#include "SdlRoad.h"

SdlRoad::SdlRoad(Renderer *renderer)
 {
	setRenderer(renderer);
	Image *roadImageStraight3Lanes=getRenderer()->loadImage(PATH_ROAD_STRAIGHT_3_LANES);
	SDL_Rect roadSpriteCutOut;
	roadSpriteCutOut.h=512;
	roadSpriteCutOut.w=900;
	roadSpriteCutOut.x=0;
	roadSpriteCutOut.y=0;
	spriteCutOut=roadSpriteCutOut;
	offset=Point(0,0);
	pos=FPoint(Point(70,0));
	roadImageStraight3Lanes->setDestinationScaleFactor(1.4);
	roadImages.push_back(roadImageStraight3Lanes);
}


SdlRoad::~SdlRoad() {
	// TODO call freeimage
}

Image* SdlRoad::getImage() const {
	return roadImages[0];
}


Point SdlRoad::getOffset() const {
	return offset;
}



SubArea SdlRoad::getSrc() const {
	return spriteCutOut;
}

void SdlRoad::addRenderables(std::vector<Renderable>& renderList) {

}

void SdlRoad::advance() {
	pos.y++;
}

/*
 * renderer uit objecten halen. Sprites laten beheren door game via map
 *
 *
 */

