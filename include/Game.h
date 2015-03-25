/*
 * Game.h
 *
 *  Created on: 9-mrt.-2015
 *      Author: Matthias
 */

#ifndef GAME_H_
#define GAME_H_

/*#include "AbstractFactory.h"
#include "SDLGpuFactory.h"*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <vector>

#include "RenderDevice.h"
#include "EntityFactory.h"
#include "GameRenderer.h"


class Game {
private:
RenderDevice *renderDevice;
GameRenderer * gameRenderer;
EntityFactory * factory;
std::vector<Entity> entitylist;
public:
	Game(std::string &render_device_name);
	RenderDevice* getRenderer() const;
	void start();
	virtual ~Game();
};




#endif /* GAME_H_ */
