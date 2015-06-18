/*
 * RoadInputComponent.h
 *
 *  Created on: 2-apr.-2015
 *      Author: Matthias
 */

#ifndef ROADINPUTCOMPONENT_H_
#define ROADINPUTCOMPONENT_H_

#include "InputComponent.h"

class Entity;

class RoadInputComponent : public InputComponent {
public:
	RoadInputComponent();
	void update(Entity& entity) override;
	virtual ~RoadInputComponent();
};

#endif /* ROADINPUTCOMPONENT_H_ */
