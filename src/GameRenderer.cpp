/*
 * GameRenderer.cpp
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#include "GameRenderer.h"
#include "Renderer.h"
#include <vector>
#include <algorithm>

GameRenderer::GameRenderer(Renderer * renderer) {
	this->renderer=renderer;
	// TODO Auto-generated constructor stub

}

GameRenderer::~GameRenderer() {
	// TODO Auto-generated destructor stub
}


bool priocompare(const Renderable *r1, const Renderable *r2) {
	return r1->getPrio() < r2->getPrio();
}

void GameRenderer::render(std::vector<Renderable*> &renderables) {
	std::sort(renderables.begin() , renderables.end(), priocompare);


	std::vector<Renderable*>::iterator currentRenderableIt = renderables.begin();
	std::vector<Renderable*>::iterator lastRenderableIt = renderables.end();
	Renderable *currentRenderable;

	while (currentRenderableIt != lastRenderableIt) {
		currentRenderable= *currentRenderableIt;
		if (currentRenderable->getImage() != NULL) {
			SubArea dest;
			dest.x = currentRenderable-> getPos().x -currentRenderable-> getOffset().x;
			dest.y = currentRenderable-> getPos().y - currentRenderable->getOffset().y;
			dest.w = (float) currentRenderable-> getSrc().w /(currentRenderable->getImage()->getDestinationScaleFactor()); //TODO uitzoeken hoe src en dest werken en hoe abstractie van SDLRect...
			dest.h = (float) currentRenderable-> getSrc().h /(currentRenderable->getImage()->getDestinationScaleFactor());
			renderer->render(*currentRenderable, dest);
			currentRenderableIt++;
		}
	}

}
