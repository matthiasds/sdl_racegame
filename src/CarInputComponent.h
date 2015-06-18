/*
 * CarInputComponent.h
 *
 *  Created on: 2-apr.-2015
 *      Author: Matthias
 */

#ifndef CARINPUTCOMPONENT_H_
#define CARINPUTCOMPONENT_H_

#include "Inputcomponent.h"

class Entity;

class CarInputComponent : public InputComponent {
public:
	CarInputComponent();
	void update(Entity& entity) override;
	virtual ~CarInputComponent();
};

#endif /* CARINPUTCOMPONENT_H_ */
