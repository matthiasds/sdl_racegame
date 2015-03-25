/*
 * Entity.cpp
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#include "Entity.h"

Entity::Entity(RenderDevice* renderer) {
	this->renderer = renderer;
}

Entity::~Entity() {
	// TODO Auto-generated destructor stub
}

Renderable* Entity::getEntityRender() const {
	return entityRender;
}
