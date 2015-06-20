/*
 * SdlSpeedRenderSystem.cpp
 *
 *  Created on: 7-apr.-2015
 *      Author: Matthias
 */

#include "SdlInfoRenderSystem.h"
#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * SdlInfoRenderSystem:
 * This is a specific Sdl System as it uses a specific Sdl Component for rendering.
 * The goal of the system is to render the damage and the speed of the playerCar in the left above corner of the gamescreen.
 */

/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */
SdlInfoRenderSystem::SdlInfoRenderSystem() {
	addComponentType<VelocityComponent>();
	addComponentType<SdlInfoRenderComponent>();
	addComponentType<DamageComponent>();
}


/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */
void SdlInfoRenderSystem::processEntity(Entity* entity) {
	drawSpeed(entity);
	drawDamage(entity);
}

/**
 * Converts the speed of the playerCar to text and renders it on the screen. (left above).
 * If the car goes in reverse. R is printed and the color of the speed indicator becomes red.
 * @param entity: entity is the container for all components, in this case the VelocityComponent containing the speed info
 */
void SdlInfoRenderSystem::drawSpeed(Entity* entity) {
	//std::sort(entitiesToUpdate.begin() , entitiesToUpdate.end(), priocompare);
	int speed = velocityMapper.get(entity)->getVelocityY();
	TTF_Font* font = sdlSpeedRenderMapper.get(entity)->getSpeedFont();
	Rect screenSize = game->getScreen();
	Rect dest;
	dest.w = screenSize.w;
	dest.h = screenSize.h;
	dest.x = screenSize.w * 0.01;
	dest.y = screenSize.h * 0.01;
	Color color = Color(255, 255, 255);
	std::ostringstream speedStr;
	if (speed < 0) {
		color = Color(255, 0, 0);
		speedStr << "R ";
	}
	speedStr << speed;
	game->getRenderer()->renderText(font, speedStr.str(), color, dest);
}


/**
 * Renders a box proportionally filled based on the damage percentage. The grade of the damage is also used for the color
 * of the box. It uses a gradient from green (no damage) to red (almost broken).
 * @param entity: entity is the container for all components, in this case the DamageComponent containing the damage info
 */
void SdlInfoRenderSystem::drawDamage(Entity* entity) {
	//std::sort(entitiesToUpdate.begin() , entitiesToUpdate.end(), priocompare);
	int damage = damageMapper.get(entity)->getDamage();
	TTF_Font* font = sdlSpeedRenderMapper.get(entity)->getOtherInfoFont();
	Rect screenSize = game->getScreen();
	Rect dest;
	int scalefactor = 10;
	dest.w = screenSize.w / scalefactor;
	dest.h = screenSize.h / scalefactor;
	dest.x = screenSize.w * 0.01;
	dest.y = screenSize.h * 0.1;
	Point leftTop = Point(screenSize.w * 0.01, screenSize.h * 0.16);
	Point rightUnder = Point(screenSize.w * 0.01 + (screenSize.w * damage) * 0.0015 , screenSize.h * 0.19);

	Color color = Color(255*damage/100, 255*(100-damage)/100, 0);
	game->getRenderer()->drawFilledRectangle(leftTop, rightUnder, color);
	rightUnder.x = screenSize.w * 0.16;
	game->getRenderer()->drawRectangle(leftTop, rightUnder, color);
	std::ostringstream speedStr;
	speedStr << "Damage: ";
	game->getRenderer()->renderText(font, speedStr.str(), Color(255, 255, 255), dest);
}


/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
SdlInfoRenderSystem::~SdlInfoRenderSystem() {
}


/**
* init: not used in this System
*/
void SdlInfoRenderSystem::init() {
}
