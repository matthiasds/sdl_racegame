/*
 * RoadGraphicsComponent.h
 *
 *  Created on: 2-apr.-2015
 *      Author: Matthias
 */
#include <vector>

#include "Image.h"
#include "Utils.h"



#ifndef ROADGRAPHICSCOMPONENT_H_
#define ROADGRAPHICSCOMPONENT_H_



#include "GraphicsComponent.h"

#define PATH_ROAD_STRAIGHT_3_LANES "textures/Asphalt-road-complete.png"

class Entity;

class RoadGraphicsComponent : public GraphicsComponent {
public:
	RoadGraphicsComponent(Renderer& renderer);
	void update(Entity& entity, Renderer& renderer) override;
	virtual ~RoadGraphicsComponent();
private:
	std::vector<Sprite*> roadSprites;
};

#endif /* ROADGRAPHICSCOMPONENT_H_ */
