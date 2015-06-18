/*
 * SdlInputSystem.cpp
 *
 *  Created on: 15-apr.-2015
 *      Author: Matthias
 */

#include "SdlInputSystem.h"


SdlInputSystem::SdlInputSystem() {
	addComponentType<InputComponent>();
}

void SdlInputSystem::init() {

}

SdlInputSystem::~SdlInputSystem() {
	// TODO Auto-generated destructor stub
}


void SdlInputSystem::processEntity(Entity* entity) {
	SDL_PollEvent (&keyEvent);

	switch (keyEvent.type){
			case SDL_KEYDOWN:
				switch (keyEvent.key.keysym.sym){
				case SDLK_UP:
					inputMapper.get(entity)->setInputs(UP);
					break;
				case SDLK_DOWN:
					inputMapper.get(entity)->setInputs(DOWN);
					break;
				case SDLK_RIGHT:
					inputMapper.get(entity)->setInputs(RIGHT);
					break;
				case SDLK_LEFT:
					inputMapper.get(entity)->setInputs(LEFT);
					break;
				default:
					inputMapper.get(entity)->setInputs(IDLE);
				}
				break;
			default:
				inputMapper.get(entity)->setInputs(IDLE);
	}
}




