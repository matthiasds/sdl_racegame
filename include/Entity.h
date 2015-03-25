/*
 * Entity.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include "RenderDevice.h"

#include "Position.h"
enum Direction { FORWARD, BACKWARD, LEFT, RIGHT, LEFT_FORWARD, RICHT_FORWARD };

class Entity {
public:
	Entity();
	Entity(RenderDevice* renderer);
	virtual ~Entity();
	Renderable* getEntityRender() const;

private:
	RenderDevice* renderer;
	Position position;
	bool alive;
protected:
	Renderable * entityRender;
	RenderDevice* getRenderer() const;

};

#endif /* ENTITY_H_ */

