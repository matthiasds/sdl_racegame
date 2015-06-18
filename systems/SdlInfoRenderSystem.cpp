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

SdlInfoRenderSystem::SdlInfoRenderSystem() {
	addComponentType<VelocityComponent>();
	addComponentType<SdlInfoRenderComponent>();
	addComponentType<DamageComponent>();
}

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



void SdlInfoRenderSystem::processEntity(Entity* entity) {
	drawSpeed(entity);
	drawDamage(entity);
}


SdlInfoRenderSystem::~SdlInfoRenderSystem() {
	// TODO Auto-generated destructor stub
}



void SdlInfoRenderSystem::init() {
}
