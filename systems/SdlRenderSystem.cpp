/*
 * SdlRenderSystem.cpp
 *
 *  Created on: 7-apr.-2015
 *      Author: Matthias
 */

#include "SdlRenderSystem.h"
#include "Game.h"

/**
 * SdlRenderSystem:
 * This is the main renderer of all objects with textures that are not background and not text.
 * This a specific SDL System. Separated from the other systems to be possibly replaced by an other renderer. This renderer is specifically
 * tied to the SdlRendercomponent. If an other renderer would be wanted this should be done in the (Sdl or other) EntityFactory which adds
 * components to the entities
 *
 * An entity in his SdlRenderComponent can contain one or multiple Sprites containing textures. All textures are rendered relative to the
 * camera viewpoint of the ReferenceCompont. This view is usually cenetered on the PlayerCar (reference)
 */


/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */
SdlRenderSystem::SdlRenderSystem() {
	addComponentType<SdlRenderComponent>();
	addComponentType<PositionComponent>();
	addComponentType<RenderReferenceComponent>();
}


/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */
void SdlRenderSystem::processEntity(Entity* entity) {

	//std::sort(entitiesToUpdate.begin() , entitiesToUpdate.end(), priocompare);

	std::vector<Au_2Drenderer::Sprite*> sprites = renderMapper.get(entity)->getSprites();
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

					if (renderMapper.get(entity)->getRenderer()->render(currentSprite) == 0) {
						renderMapper.get(entity)->setOnScreen(true);
					}
					else {
						renderMapper.get(entity)->setOnScreen(false);
					}
				}

			}
	}

}

/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
SdlRenderSystem::~SdlRenderSystem() {
}


/**
* init: not used in this System
*/
void SdlRenderSystem::init() {
}
