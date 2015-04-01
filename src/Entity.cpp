/*
 * Entity.cpp
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#include "Entity.h"

Entity::Entity() :
 pos()
, prio(0) {

}

Entity::~Entity() {
	// TODO Auto-generated destructor stub
}



FPoint Entity::getPos() const {
	return pos;
}


uint32_t Entity::getPrio() const {
	return prio;
}

void Entity::setRenderer(Renderer* renderer) {
	this->renderer=renderer;
}

Renderer* Entity::getRenderer() const {
	return renderer;
}

