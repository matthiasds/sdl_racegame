/*
 * BackGroundGraphicsComponent.h
 *
 *  Created on: 2-apr.-2015
 *      Author: Matthias
 */
#include <vector>

#include "Image.h"
#include "Utils.h"



#ifndef BACKGROUNDGRAPHICSCOMPONENT_H_
#define BACKGROUNDGRAPHICSCOMPONENT_H_



#include "RenderComponent.h"
#include "IP"

#define GRASS "textures/grass.jpg"

class Entity;

class BackGroundRenderProcess : public IProcess {
public:
	BackGroundRenderProcess(Renderer& renderer);
	void update() override;
	virtual ~BackGroundRenderProcess();
private:
	std::vector<IRenderable> targets;
};

#endif /* BACKGROUNDGRAPHICSCOMPONENT_H_ */
