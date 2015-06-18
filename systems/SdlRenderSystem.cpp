/*
 * RenderSystem.cpp
 *
 *  Created on: 7-apr.-2015
 *      Author: Matthias
 */

#include "SdlRenderSystem.h"
#include "Game.h"

SdlRenderSystem::SdlRenderSystem() {
	addComponentType<RenderComponent>();
	addComponentType<PositionComponent>();
	addComponentType<RenderReferenceComponent>();
}

/* renders the entities  with a RenderComponent (containing 1 or multiple Sprites) and PositionComponent (Position) with a position relative to a ReferenceComponenet (Entity) */

void SdlRenderSystem::processEntity(Entity* entity) {

	//std::sort(entitiesToUpdate.begin() , entitiesToUpdate.end(), priocompare);

	std::vector<Sprite*> sprites = renderMapper.get(entity)->getSprites();
	Entity* reference = renderReferenceMapper.get(entity)->getReference();
	Point referenceScreenLocation = renderReferenceMapper.get(reference)->getReferenceScreenLocation();
	FPoint referencePosition = positionMapper.get(reference)->getPosition();
	std::vector<FPoint>& enityPositionVector = positionMapper.get(entity)->getPositionVector();

	//render all different sprites in the sprite vector of the entity rendercomponent
	for (auto const &currentSprite : sprites) {
		if (currentSprite != NULL) {
				for (auto const &currentposition : enityPositionVector) {
					float positionDifferenceY = currentposition.y - referencePosition.y + referenceScreenLocation.y;

					currentSprite->setDestY(-positionDifferenceY);
					currentSprite->setDestX(currentposition.x);
					//std::cout << "render car at game coordinate " << enityPosition.y << " to render coordinate (window) at " << spritePosition.y << std::endl;

					if (game->getRenderer()->render(currentSprite) == 0) {
						renderMapper.get(entity)->setOnScreen(true);
					}
					else {
						renderMapper.get(entity)->setOnScreen(false);
					}
				}

			}
	}

}

SdlRenderSystem::~SdlRenderSystem() {
	// TODO Auto-generated destructor stub
}



void SdlRenderSystem::init() {
}
