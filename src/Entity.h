/*
 * Entity.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include "Renderer.h"
#include "Utils.h"


class Entity : public Renderable {
public:
	Entity();
	FPoint getPos() const override;
	uint32_t getPrio() const override;
	Renderer* getRenderer() const override;
	virtual ~Entity();

private:
	Renderer* renderer;
	bool alive;
protected:
	virtual void setRenderer(Renderer* renderer);
	FPoint pos;
	uint32_t prio;
};

#endif /* ENTITY_H_ */

