/*
 * CarGraphicsComponent.h
 *
 *  Created on: 2-apr.-2015
 *      Author: Matthias
 */


#ifndef CARGRAPHICSCOMPONENT_H_
#define CARGRAPHICSCOMPONENT_H_

#include "Utils.h"
#include "GraphicsComponent.h"
#include <vector>

class Sprite;






class Entity;
class Renderer;

#define PATH_CAR1 "textures/Gray_Mustang_GT.png"
#define PATH_CAR2 "textures/Metallic_Red_Jeep_Liberty.png"

class CarGraphicsComponent : public GraphicsComponent{
public:
	CarGraphicsComponent(Renderer& renderer);
	void update(Entity& entity, Renderer& renderer) override;
	virtual ~CarGraphicsComponent();
private:
	std::vector<Sprite*> carSprites;
};

#endif /* CARGRAPHICSCOMPONENT_H_ */
