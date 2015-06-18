/*
 * BackGroundRenderProcess.cpp
 *
 *  Created on: 2-apr.-2015
 *      Author: Matthias
 */

#include "BackGroundRenderProcess.h"
#include "Renderer.h"
#include "Entity.h"


BackGroundRenderProcess::BackGroundRenderProcess(Renderer& renderer) {
	Image *bakgroundImage=renderer.loadImage(GRASS);
	int originalImagesizeW = bakgroundImage->getWidth();
	int originalImagesizeH = bakgroundImage->getHeight();
	SubArea screenSize=renderer.getContextSize();
	int windowsizeW = screenSize.w;
	int windowsizeH = screenSize.h;
	int scaledImageSizeW = windowsizeW/4;
	int scaledImageSizeH = originalImagesizeH *((float) scaledImageSizeW/originalImagesizeW);//keep aspect ratio of original
	bakgroundImage = bakgroundImage->resize(scaledImageSizeW,scaledImageSizeH);
	int numberOfImagesNeededtoFillHeight = (windowsizeH/scaledImageSizeH)+2;
	for (int i=0; i<=numberOfImagesNeededtoFillHeight; i++)
	{
		Sprite *backgroundSpriteL = bakgroundImage->createSprite(true);
		backgroundSpriteL->setOffset(0,0);
		backgroundSprites.push_back(backgroundSpriteL);
		Sprite *backgroundSpriteR = bakgroundImage->createSprite(true);
		backgroundSpriteR->setOffset(-windowsizeW+scaledImageSizeW,0);
		backgroundSprites.push_back(backgroundSpriteR);
	}
}

BackGroundRenderProcess::~BackGroundRenderProcess() {
	// TODO Auto-generated destructor stub
}

void BackGroundRenderProcess::update() {
	for(std::size_t i = 0; i < backgroundSprites.size(); i++) {
		Point Offset = backgroundSprites[i]->getOffset();
		int imageH = backgroundSprites[i]->getGraphicsHeight();
		SubArea screenSize=renderer.getContextSize();
		int windowsizeH = screenSize.h;
		Offset.y= + imageH - (((entity.getPos().y) - (i/2 * imageH)) % (windowsizeH + imageH) );
		backgroundSprites[i]->setOffset(Offset);
		renderer.render(backgroundSprites[i]);
	}
}
