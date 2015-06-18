/*
 * RoadGraphicsComponent.cpp
 *
 *  Created on: 2-apr.-2015
 *      Author: Matthias
 */

#include "RoadGraphicsComponent.h"
#include "Renderer.h"
#include "Entity.h"


RoadGraphicsComponent::RoadGraphicsComponent(Renderer& renderer) {
	Image *roadImageStraight3Lanes=renderer.loadImage(PATH_ROAD_STRAIGHT_3_LANES);
	int originalImagesizeW = roadImageStraight3Lanes->getWidth();
	int originalImagesizeH = roadImageStraight3Lanes->getHeight();
	SubArea screenSize=renderer.getContextSize();
	int windowsizeW = screenSize.w;
	int windowsizeH = screenSize.h;
	int scaledImageSizeW = windowsizeW/1.7;
	int scaledImageSizeH = originalImagesizeH *((float) scaledImageSizeW/originalImagesizeW);//keep aspect ratio of original
	roadImageStraight3Lanes = roadImageStraight3Lanes->resize(scaledImageSizeW,scaledImageSizeH);
	Point offset=Point(-(windowsizeW-scaledImageSizeW)/2, 0); //center

	int numberOfImagesNeededtoFillHeight = (windowsizeH/scaledImageSizeH)+2;
	for (int i=0; i<=numberOfImagesNeededtoFillHeight; i++)
	{
		Sprite *roadSprite = roadImageStraight3Lanes->createSprite(true);
		roadSprite->setOffset(offset);
		roadSprites.push_back(roadSprite);
	}
}

RoadGraphicsComponent::~RoadGraphicsComponent() {
	// TODO Auto-generated destructor stub
}

void RoadGraphicsComponent::update(Entity& entity, Renderer& renderer) {

	for(std::size_t i = 0; i < roadSprites.size(); i++) {
		Point Offset = roadSprites[i]->getOffset();
		int imageH = roadSprites[i]->getGraphicsHeight();
		SubArea screenSize=renderer.getContextSize();
		int windowsizeH = screenSize.h;
		Offset.y= + imageH - (((entity.getPos().y) - (i * imageH)) % (windowsizeH + imageH) );
		roadSprites[i]->setOffset(Offset);
		renderer.render(roadSprites[i]);
	}
}



