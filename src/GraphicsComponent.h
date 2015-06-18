/*
 * GraphicsComponent.h
 *
 *  Created on: 1-apr.-2015
 *      Author: Matthias
 */

#ifndef GRAPHICSCOMPONENT_H_
#define GRAPHICSCOMPONENT_H_

class Game;
class Entity;
class Renderer;

class GraphicsComponent {
public:
	virtual ~GraphicsComponent() {};
	virtual void update(Entity& entity, Renderer& renderer) = 0;
};

#endif /* GRAPHICSCOMPONENT_H_ */
