/*
 * SdlInputSystem.cpp
 *
 *  Created on: 15-apr.-2015
 *      Author: Matthias
 */

#include "SdlInputSystem.h"

/**
 * SdlInputSystem:
 * this is a specific InputSystem split from other systems because it is SDL only and could be replaced
 * by other InputSystems in the other Systems that interact with it
 * It converts the SDL Key presses in states used by other systems, to make these independent of SDL.
 */


/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */
SdlInputSystem::SdlInputSystem() {
	addComponentType<InputComponent>();
}


/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */
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

/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
SdlInputSystem::~SdlInputSystem() {
}

/**
* init: not used in this System
*/
void SdlInputSystem::init() {
}






