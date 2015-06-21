/*
 * SdlMovingEntityDebugSystem.cpp
 *
 *  Created on: 10-may.-2015
 *      Author: Matthias
 */

#include "SdlMovingEntityDebugSystem.h"
#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Utils.h"

/**
 * SdlMovingEntityDebugSystem
 * This is an optionalComponent only added by the EntityFactory to the entities if the debug option is used as argument when starting the application
 * This is an SDL specific component as it directly renders the information of all Entities on the entities itself.
 * It draws all Entity properties like, speed, damage, wantedLane, currentLane, PositionX, PositionY, collision information when this occurs...
 *
 * It is applied to all moving entities. So PlayerCar as well as EnemyCars. The normal debugger can be used in conditional break for ex
 * on the parameter entity->id==4. But this SdlMovingEntityDebugSystem is faster and more intuitive to observer and debug interacting entities.
 */


/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */
SdlMovingEntityDebugSystem::SdlMovingEntityDebugSystem() {
	addComponentType<PositionComponent>();
	addComponentType<VelocityComponent>();
	addComponentType<SdlDebugComponent>();
	addComponentType<CollisionComponent>();
	addComponentType<RenderReferenceComponent>();
	addComponentType<DamageComponent>();
	addComponentType<LaneComponent>();
}

/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */
void SdlMovingEntityDebugSystem::processEntity(Entity* entity) {
	if (Utils::getInstance().isDebuggingEnabled()) {
		if (collisionMapper.get(entity)->getCollisionStatus().collisionHandlingState != NO_COLLISION){
			drawCollisionDebugInfo(entity); //draw collision debug info
		}
		drawSpeedDebugInfo(entity);
		drawPositionDebugInfo(entity);
		drawDamageDebugInfo(entity);
		drawLaneDebugInfo(entity);
		drawIdInfo(entity);
	}

}

/**
 * Renders the entity ID number on the car
 * @param entity: entity of which the ID is rendered
 */
void SdlMovingEntityDebugSystem::drawIdInfo(Entity* entity) {
	int id = entity->getId();
	TTF_Font * font = debugMapper.get(entity)->getFont();
	Entity* reference = renderReferenceMapper.get(entity)->getReference();
	FPoint referencePosition = positionMapper.get(reference)->getPosition();
	Point referenceScreenLocation = renderReferenceMapper.get(reference)->getReferenceScreenLocation();
	FPoint myPosition = positionMapper.get(entity)->getPosition();

	Rect entityCollisionBox = collisionMapper.get(entity)->getCollisionRect();

	Rect dest;

	debugMapper.get(entity)->setColor(255,255,255);
	Color color = debugMapper.get(entity)->getColor();
	dest.w = entityCollisionBox.w*1.9;
	dest.h = entityCollisionBox.h*0.1;
	dest.x = myPosition.x - entityCollisionBox.w*0.4;
	dest.y = referencePosition.y - referenceScreenLocation.y - myPosition.y + entityCollisionBox.h *0.4;

	std::ostringstream idStr;

	idStr << "ID: " << id;
	game->getRenderer()->renderText(font, idStr.str(), color, dest );
}


/**
 * Renders the X and Y speed on the entity in color blue
 * @param entity: entity is the container of all components, so this is the access to all wanted information
 */
void SdlMovingEntityDebugSystem::drawSpeedDebugInfo(Entity* entity) {
	int velocityY = velocityMapper.get(entity)->getVelocityY();
	int velocityX = velocityMapper.get(entity)->getVelocityX();
	TTF_Font * font = debugMapper.get(entity)->getFont();
	Entity* reference = renderReferenceMapper.get(entity)->getReference();
	FPoint referencePosition = positionMapper.get(reference)->getPosition();
	Point referenceScreenLocation = renderReferenceMapper.get(reference)->getReferenceScreenLocation();
	FPoint myPosition = positionMapper.get(entity)->getPosition();

	Rect entityCollisionBox = collisionMapper.get(entity)->getCollisionRect();

	Rect dest;

	debugMapper.get(entity)->setColor(0,0,255);
	Color color = debugMapper.get(entity)->getColor();
	dest.w = entityCollisionBox.w*1.9;
	dest.h = entityCollisionBox.h*0.1;
	dest.x = myPosition.x - entityCollisionBox.w*0.4;
	dest.y = referencePosition.y - referenceScreenLocation.y - myPosition.y - entityCollisionBox.h *0.4;

	std::ostringstream speedStr;

	speedStr << "speedX: " << velocityX;
	game->getRenderer()->renderText(font, speedStr.str(), color, dest );

	speedStr.clear();
	speedStr.str("");
	dest.y -=entityCollisionBox.h*0.05;
	speedStr << "speedY: " << velocityY;
	game->getRenderer()->renderText(font, speedStr.str(), color, dest );
}

/**
 * Renders the X and Y position on the entity in color cyan
 * @param entity: entity is the container of all components, so this is the access to all wanted information
 */
void SdlMovingEntityDebugSystem::drawPositionDebugInfo(Entity* entity) {
	TTF_Font * font = debugMapper.get(entity)->getFont();
	Entity* reference = renderReferenceMapper.get(entity)->getReference();
	FPoint referencePosition = positionMapper.get(reference)->getPosition();
	Point referenceScreenLocation = renderReferenceMapper.get(reference)->getReferenceScreenLocation();
	FPoint myPosition = positionMapper.get(entity)->getPosition();

	Rect entityCollisionBox = collisionMapper.get(entity)->getCollisionRect();

	Rect dest;

	debugMapper.get(entity)->setColor(0,255,255);
	Color color = debugMapper.get(entity)->getColor();
	dest.w = entityCollisionBox.w*1.9;
	dest.h = entityCollisionBox.h*0.1;
	dest.x = myPosition.x - entityCollisionBox.w*0.4;
	dest.y = referencePosition.y - referenceScreenLocation.y - myPosition.y - entityCollisionBox.h *0.3;

	std::ostringstream speedStr;

	speedStr << "posX: " << myPosition.x;
	game->getRenderer()->renderText(font, speedStr.str(), color, dest );

	speedStr.clear();
	speedStr.str("");
	dest.y -=entityCollisionBox.h*0.05;
	speedStr << "posY: " << myPosition.y;
	game->getRenderer()->renderText(font, speedStr.str(), color, dest );
}

/**
 * Renders the damage percentage on the entity in color magenta
 * @param entity: entity is the container of all components, so this is the access to all wanted information
 */
void SdlMovingEntityDebugSystem::drawDamageDebugInfo(Entity* entity) {
	int damage = damageMapper.get(entity)->getDamage();
	TTF_Font * font = debugMapper.get(entity)->getFont();
	Entity* reference = renderReferenceMapper.get(entity)->getReference();
	FPoint referencePosition = positionMapper.get(reference)->getPosition();
	Point referenceScreenLocation = renderReferenceMapper.get(reference)->getReferenceScreenLocation();
	FPoint myPosition = positionMapper.get(entity)->getPosition();

	Rect entityCollisionBox = collisionMapper.get(entity)->getCollisionRect();

	Rect dest;

	debugMapper.get(entity)->setColor(255,0,255);
	Color color = debugMapper.get(entity)->getColor();
	dest.w = entityCollisionBox.w*1.9;
	dest.h = entityCollisionBox.h*0.1;
	dest.x = myPosition.x - entityCollisionBox.w*0.4;
	dest.y = referencePosition.y - referenceScreenLocation.y - myPosition.y - entityCollisionBox.h *0.2;

	std::ostringstream speedStr;

	speedStr << "Damage: " << damage;
	game->getRenderer()->renderText(font, speedStr.str(), color, dest );
}


/**
 * Renders the wanted and current lane numbers on the entity in the color orange
 * @param entity: entity is the container of all components, so this is the access to all wanted information
 */
void SdlMovingEntityDebugSystem::drawLaneDebugInfo(Entity* entity) {
	int wantedLane = laneMapper.get(entity)->getWantedLane();
	int currentLane = laneMapper.get(entity)->getCurrentLane();
	TTF_Font * font = debugMapper.get(entity)->getFont();
	Entity* reference = renderReferenceMapper.get(entity)->getReference();
	FPoint referencePosition = positionMapper.get(reference)->getPosition();
	Point referenceScreenLocation = renderReferenceMapper.get(reference)->getReferenceScreenLocation();
	FPoint myPosition = positionMapper.get(entity)->getPosition();

	Rect entityCollisionBox = collisionMapper.get(entity)->getCollisionRect();

	Rect dest;

	debugMapper.get(entity)->setColor(255,125,125);
	Color color = debugMapper.get(entity)->getColor();
	dest.w = entityCollisionBox.w*1.9;
	dest.h = entityCollisionBox.h*0.1;
	dest.x = myPosition.x - entityCollisionBox.w*0.4;
	dest.y = referencePosition.y - referenceScreenLocation.y - myPosition.y + entityCollisionBox.h *0.2;

	std::ostringstream laneStr;

	laneStr << "LaneW: " << wantedLane;
	game->getRenderer()->renderText(font, laneStr.str(), color, dest );

	laneStr.clear();
	laneStr.str("");
	dest.y +=entityCollisionBox.h*0.05;
	laneStr << "LaneC: " << currentLane;
	game->getRenderer()->renderText(font, laneStr.str(), color, dest );
}

/**
 * Renders a green box around the 2 entities colliding representing the collisionBox. A red cross indicates point of impact
 * The SpeedOfimpact in X and Y direction of the last collision are also rendered in red.
 * @param entity: entity is the container of all components, so this is the access to all wanted information
 */
void SdlMovingEntityDebugSystem::drawCollisionDebugInfo(Entity* entity) {

	//get info to be able to draw on the correct place
	Entity* reference = renderReferenceMapper.get(entity)->getReference();
	FPoint referencePosition = positionMapper.get(reference)->getPosition();
	Point referenceScreenLocation = renderReferenceMapper.get(reference)->getReferenceScreenLocation();

	//draw collison box for debugging
	Rect entityCollisionBox = collisionMapper.get(entity)->getCollisionRect();
	FPoint myPosition = positionMapper.get(entity)->getPosition();
	entityCollisionBox.x = myPosition.x;
	entityCollisionBox.y = myPosition.y;
	Point collisionBoxLeftUnder = Point(entityCollisionBox.x - entityCollisionBox.w/2, referencePosition.y - referenceScreenLocation.y - entityCollisionBox.y - entityCollisionBox.h/2 );
	Point collisionBoxRightAbove = Point(entityCollisionBox.x + entityCollisionBox.w/2, referencePosition.y - referenceScreenLocation.y - entityCollisionBox.y + entityCollisionBox.h/2 );
	game->getRenderer()->drawRectangle(collisionBoxLeftUnder, collisionBoxRightAbove, Color(0,255,0) );

	//draw collision point of impact
	CollisionStatus& collisionStatus = collisionMapper.get(entity)->getCollisionStatus();
	int renderCoordinateY =  referencePosition.y - referenceScreenLocation.y-collisionStatus.pointOfImpact.y;
	game->getRenderer()->drawLine(collisionStatus.pointOfImpact.x -5,  renderCoordinateY,  collisionStatus.pointOfImpact.x +5,  renderCoordinateY, Color(255,0,0) );
	game->getRenderer()->drawLine(collisionStatus.pointOfImpact.x,  renderCoordinateY-5,  collisionStatus.pointOfImpact.x,  renderCoordinateY+5, Color(255,0,0) );


	Rect dest;
	debugMapper.get(entity)->setColor(255,0,0);
	Color color = debugMapper.get(entity)->getColor();
	TTF_Font * font = debugMapper.get(entity)->getFont();

	//draw text speedOfImpact
	dest.w = entityCollisionBox.w*1.9;
	dest.h = entityCollisionBox.h*0.1;
	dest.x = myPosition.x - entityCollisionBox.w*0.4;
	dest.y = referencePosition.y - referenceScreenLocation.y - myPosition.y - entityCollisionBox.h *0.1;

	std::ostringstream speedStr;

	speedStr << "SOImpactX: " << collisionStatus.speedOfImpactX;
	game->getRenderer()->renderText(font, speedStr.str(), color, dest );

	dest.y -= entityCollisionBox.h *0.05;
	speedStr.clear();
	speedStr.str("");
	speedStr << "SOImpactY: " << collisionStatus.speedOfImpactY;
	game->getRenderer()->renderText(font, speedStr.str(), color, dest );


}

/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
SdlMovingEntityDebugSystem::~SdlMovingEntityDebugSystem() {
	// TODO Auto-generated destructor stub
}


/**
* init: not used in this System
*/
void SdlMovingEntityDebugSystem::init() {
}
