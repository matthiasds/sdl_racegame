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

SdlMovingEntityDebugSystem::SdlMovingEntityDebugSystem() {
	addComponentType<PositionComponent>();
	addComponentType<VelocityComponent>();
	addComponentType<SdlDebugComponent>();
	addComponentType<CollisionComponent>();
	addComponentType<RenderReferenceComponent>();
	addComponentType<DamageComponent>();
}

void SdlMovingEntityDebugSystem::processEntity(Entity* entity) {
	if (Utils::getInstance().isDebuggingEnabled()) {
		if (collisionMapper.get(entity)->getCollisionStatus().collisionHandlingState != NO_COLLISION){
			drawCollisionDebugInfo(entity); //draw collision debug info
		}
		drawSpeedDebugInfo(entity);
		drawPositionDebugInfo(entity);
		drawDamageDebugInfo(entity);
	}

}

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


SdlMovingEntityDebugSystem::~SdlMovingEntityDebugSystem() {
	// TODO Auto-generated destructor stub
}



void SdlMovingEntityDebugSystem::init() {
}
