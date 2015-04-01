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

#include "Renderer.h"
#include "EntityFactory.h"
#include "GameRenderer.h"


class Road;


class Game {
private:
	Road *road; //temp
Renderer *renderDevice;
GameRenderer * gameRenderer;
EntityFactory * factory;
std::vector<Renderable*> renderList;  //list of imlemented class renderable +- composite design pattern
std::vector<Entity*> entityList;
public:
	Game(std::string &render_device_name);
	Renderer* getRenderer() const;
	void start();
	virtual ~Game();
};




#endif /* GAME_H_ */
