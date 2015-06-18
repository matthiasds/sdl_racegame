/*
 * SdlInputSystem.h
 *
 *  Created on: 15-apr.-2015
 *      Author: Matthias
 */

#ifndef SDLINPUTSYSTEM_H_
#define SDLINPUTSYSTEM_H_

#include <bitset>
#include "BitSize.h"
#include "System.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "../components/InputComponent.h"
#include <SDL2/SDL.h>


class SdlInputSystem : public System {
public:
	SdlInputSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~SdlInputSystem();
private:
	ComponentMapper<InputComponent> inputMapper;
	SDL_Event keyEvent;
};

#endif /* SDLINPUTSYSTEM_H_ */
