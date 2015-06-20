/*
 * BackgroundRenderSystem.cpp
 *
 *  Created on: 13-apr.-2015
 *      Author: Matthias
 */

#include "BackgroundRenderSystem.h"
#include "Game.h"
#include "Sprite.h"


/**
 *  BackgroundRenderSystem:
 *  Takes care of background rendering by using tiles. this makes it possible to make use of multiple tiles for road or
 *  border next to road. A reference is set to be followed (Camera) Usually this should be the player car.
 *  The same textures are used and generated a few tiles before the reference (player Car) and remembered for a certain
 *  history. This history makes it possible to drive in reverse, but because it has a limitted history length. On a certain
 *  point going further in reverse should be blocked or you drive into a black hole.
 */



/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */
BackgroundRenderSystem::BackgroundRenderSystem() {
	addComponentType<BackgroundRenderComponent>();
	addComponentType<RenderReferenceComponent>();
	addComponentType<PositionComponent>();
	addComponentType<SdlRenderComponent>();
}

/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */
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



	/* orientation in this phase is higher Y value is the background to come when moving forward */
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
					std::vector<Point>& spriteOffsetVector = currentTileSprite->getOffset();
					Point offset = spriteOffsetVector.at(0); //load inital offset
					offset.y += tilePosition.y - backgroundPosition.y;
					if (offset.y != 0) { //if it is another tile then the first tile
						spriteOffsetVector.push_back(offset);
					}
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
		//make lowest tile highest tile
		Sprite * lowestYTileSprite = backgroundSprites.at(lowestYTile->getTextureId());
		int lowestYTileHeight = lowestYTileSprite->getTextureRenderBox().h;//lowestYTileSprite->getGraphicsHeight()*lowestYTileSprite->getScale();
		lowestYTile->getPosition().y = highestYTile->getPosition().y + lowestYTileHeight;
		highestYTile=lowestYTile; //change pointers
	}

	backgroundRenderMapper.get(entity)->setReferenceTile(referenceTile);
	backgroundRenderMapper.get(entity)->setHighestTile(highestYTile);

}



/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
BackgroundRenderSystem::~BackgroundRenderSystem() {
}

/**
 * init: not used in this System
 */
void BackgroundRenderSystem::init() {

}
