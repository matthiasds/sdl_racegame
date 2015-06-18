 /*
 * BackgroundRenderComponent.h
 *
 *  Created on: 6-apr.-2015
 *      Author: Matthias
 */

#ifndef BACKGROUNDRENDERCOMPONENT_H_
#define BACKGROUNDRENDERCOMPONENT_H_

#include "IComponent.h"
#include "Renderer.h"
#include "BackgroundTile.h"
#include "Sprite.h"

class BackgroundRenderComponent : public IComponent{
public:


	BackgroundRenderComponent(int defaulttextureID, int tileHistoryLength, Rect renderArea) {

//
//		Image* image=renderer->loadImage(texturePath);
//		int originalImagesizeW = image->getWidth();
//		int originalImagesizeH = image->getHeight();
//		Rect screenSize=renderer->getContextSize();
//		int windowsizeW = screenSize.w;
//		int windowsizeH = screenSize.h;
//
//		int scaledImageSizeW = windowsizeW/scale;
//		int scaledImageSizeH = originalImagesizeH *((float) scaledImageSizeW/originalImagesizeW);//scale related to windowsize
//		image = image->resize(scaledImageSizeW,scaledImageSizeH); //keep aspect ratio
//		Point offset=Point(-(windowsizeW-scaledImageSizeW)/2, -(windowsizeH - scaledImageSizeH)); //center
//
//		Sprite* sprite = image->createSprite(true, renderWindow);
//		sprite->setOffset(offset);

		//store renderArea info;
		this->renderArea = renderArea;
	
		for (int i = 0; i <= tileHistoryLength; i++)
		{
			BackgroundTile* backgroundTile = new BackgroundTile(defaulttextureID, Point(0,0), ROAD_3_LANES);
//			backgroundTile->getPosition().y+=numberOfImagesNeededtoFillHeight*scaledImageSizeH; //tiles above//
//			backgroundTile->getPosition().y-=i*scaledImageSizeH; //tiles history
			backgroundTiles.push_back(backgroundTile);
		}
	}

	virtual ~BackgroundRenderComponent() {
		std::vector<BackgroundTile*>::iterator it;
		for (it = backgroundTiles.begin(); it != backgroundTiles.end(); ++it) {
					BackgroundTile* backgroundTile = *it;
					delete backgroundTile;
		}
	}


	std::vector<BackgroundTile*>& getBackgroundTiles() {
		return backgroundTiles;
	}

	BackgroundTile* getReferenceTile() {
		return referenceTile;
	}

	void setReferenceTile(BackgroundTile* referenceTile) {
		this->referenceTile = referenceTile;
	}

	BackgroundTile* getHighestTile(){
		return highestTile;
	}

	void setHighestTile(BackgroundTile* highestTile) {
		this->highestTile = highestTile;
	}

	const Rect& getRenderArea() const {
		return renderArea;
	}

private:
	std::vector<BackgroundTile*> backgroundTiles;
	//the tile on which the reference entity is at the moment.
	BackgroundTile* referenceTile;
	//the most upper tile generated
	BackgroundTile* highestTile;
	Rect renderArea;
};

#endif /* BACKGROUNDRENDERCOMPONENT_H_ */
