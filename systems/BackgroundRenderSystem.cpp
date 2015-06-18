/*
 * BackgroundRenderSystem.cpp
 *
 *  Created on: 13-apr.-2015
 *      Author: Matthias
 */

#include "BackgroundRenderSystem.h"
#include "Game.h"
#include "Sprite.h"

BackgroundRenderSystem::BackgroundRenderSystem() {
	addComponentType<BackgroundRenderComponent>();
	addComponentType<RenderReferenceComponent>();
	addComponentType<PositionComponent>();
	addComponentType<RenderComponent>();
}

void BackgroundRenderSystem::processEntity(Entity* entity) {
	std::vector<BackgroundTile*> backgroundTiles = backgroundRenderMapper.get(entity)->getBackgroundTiles();
	Entity* reference = renderReferenceMapper.get(entity)->getReference();
	FPoint referenceEntityPosition = positionMapper.get(reference)->getPosition();
	FPoint backgroundPosition = positionMapper.get(entity)->getPosition();
	std::vector<Sprite*> backgroundSprites = renderMapper.get(entity)->getSprites();
	//float positionDifferenceY = referenceEntityPosition.y - backgroundPosition.y;

	//get the dimensions of the part of the window the background should be rendered
	Rect screenSize=backgroundRenderMapper.get(entity)->getRenderArea();
	int windowsizeW = screenSize.w;
	int windowsizeH = screenSize.h;



	/*orientation in this phase is higher Y value is the background to come when moving forward */
	/* later on at rendering the lowest border of the screen is the highest Y value */

	BackgroundTile* highestYTile = *(backgroundTiles.begin());
	BackgroundTile* lowestYTile = *(backgroundTiles.begin());
	static BackgroundTile* referenceTile = *(backgroundTiles.begin());
	std::vector<BackgroundTile*>::iterator currentTile;
	//iterate over all used tiles


	//update position
	backgroundPosition.y = referenceTile->getPosition().y;
	positionMapper.get(entity)->setPosition(backgroundPosition);

	//clear all offset positions to fill them  again
	for (auto &currentBackgroundSprite : backgroundSprites ) {
		currentBackgroundSprite->clearExtraRenderOffsets();
	}

	for (currentTile = backgroundTiles.begin(); currentTile != backgroundTiles.end(); ++currentTile)
	{
		FPoint tilePosition = (*currentTile)->getPosition();
		//tilePosition.y-=positionDifferenceY;
		//update min and max tileposition
		if (tilePosition.y <= lowestYTile->getPosition().y) {
			lowestYTile=(*currentTile); //set minYTile to current tile
		}
		if (tilePosition.y >= highestYTile->getPosition().y) {
			highestYTile=(*currentTile); //set minYTile to current tile
		}
		//update tile position where reference is on
		//check for all tiles under the reference if the difference with the reference is smaller then the current difference
		if (tilePosition.y <= referenceTile->getPosition().y && ((*currentTile)->getPosition().y - tilePosition.y) < (referenceTile->getPosition().y - tilePosition.y)) {
			referenceTile=(*currentTile); //set ReferenceTile to current tile
		}

		if ((*currentTile)->getTextureId() < (int) backgroundSprites.size())
		{
		Sprite * currentTileSprite = backgroundSprites.at((*currentTile)->getTextureId());

			if (currentTileSprite != NULL ) //sprite exist in renderComponent with this textureId?
			{
				//float renderPositionY = tilePosition.y;
				//if (renderPositionY >= -windowsizeH && renderPositionY <= windowsizeH) {  //position to render is in the screen area?
					//render current sprite with updated position;
					//backgroundPosition.y = renderPositionY;
					std::vector<Point>& spriteOffsetVector = currentTileSprite->getOffset();
					Point offset = spriteOffsetVector.at(0); //load inital offset
					offset.y += tilePosition.y - backgroundPosition.y;
					if (offset.y != 0) { //if it is another tile then the first tile
						spriteOffsetVector.push_back(offset);
					}





					//currentTileSprite->setDestY(renderPositionY);
					//currentTileSprite->setDestX(backgroundPosition.x);
					//std::cout << "render road tile " << currentTile -backgroundTiles.begin() <<" at game coordinate " << tilePosition.y << " to render coordinate (window) at " << offset.y << std::endl;
					//game->getRenderer()->render(currentTileSprite);
				//}
			}
			else {
				std::cerr << "Sprite with spriteId  " << (*currentTile)->getTextureId() << " not available in rendercomponent " << std::endl;
			}
		} else {
			std::cerr << "Sprite with spriteId  " << (*currentTile)->getTextureId() << " not available in rendercomponent " << std::endl;
		}
	}


	//check if new tile loading is necessary
	if ((highestYTile->getPosition().y - referenceEntityPosition.y) <= windowsizeH) //new tile is less then 1 window size ahead
	{
		//std::cout << "load new tile " << std::endl;
		//make lowest tile highest tile
		Sprite * lowestYTileSprite = backgroundSprites.at(lowestYTile->getTextureId());
		int lowestYTileHeight = lowestYTileSprite->getTextureRenderBox().h;//lowestYTileSprite->getGraphicsHeight()*lowestYTileSprite->getScale();
		lowestYTile->getPosition().y = highestYTile->getPosition().y + lowestYTileHeight;
		highestYTile=lowestYTile; //change pointers
	}

	backgroundRenderMapper.get(entity)->setReferenceTile(referenceTile);
	backgroundRenderMapper.get(entity)->setHighestTile(highestYTile);

}




//	for(std::size_t i = 0; i < sprites.size(); i++) {
//		std::vector<Sprite*> sprites = renderMapper.get(entity)->getSprites();
//		Point Offset = sprites[i]->getOffset();
//		int imageH = sprites[i]->getGraphicsHeight();
//		SubArea screenSize=game->getRenderer()->getContextSize();
//		int windowsizeH = screenSize.h;
//		Offset.y= + imageH - (((entity->getPos().y) - (i * imageH)) % (windowsizeH + imageH) );
//		sprites[i]->setOffset(Offset);
//		game->getRenderer()->render(sprites[i]);
//	}


//template<typename component_type>
//void BackgroundRenderSystem::addComponentType() {
//      //Add Bits to typeflags
//      typeFlags |= ComponentTypeManager::getBit<component_type>();
//}



BackgroundRenderSystem::~BackgroundRenderSystem() {
	// TODO Auto-generated destructor stub
}

void BackgroundRenderSystem::init() {
	//count the number of textures currently in the rendercomponent and add enough so the whole renderArea can be filled with it.


}
