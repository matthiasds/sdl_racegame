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

void GameRenderer::render(std::vector<GraphicsComponent> &renderables) {
	std::sort(renderables.begin() , renderables.end(), priocompare);


	std::vector<GraphicsComponent>::iterator currentRenderableIt = renderables.begin();
	std::vector<GraphicsComponent>::iterator lastRenderableIt = renderables.end();
	GraphicsComponent currentGraphicsComponent;
	while (currentRenderableIt != lastRenderableIt) {
		currentGraphicsComponent= *currentRenderableIt;

		if (currentGraphicsComponent->getImage() != NULL) {
			SubArea dest;
			dest.x = currentGraphicsComponent-> getPos().x -currentGraphicsComponent-> getOffset().x;
			dest.y = currentGraphicsComponent-> getPos().y - currentGraphicsComponent->getOffset().y;
			dest.w = (float) currentGraphicsComponent-> getSrc().w /(currentGraphicsComponent->getImage()->getDestinationScaleFactor()); //TODO uitzoeken hoe src en dest werken en hoe abstractie van SDLRect...
			dest.h = (float) currentGraphicsComponent-> getSrc().h /(currentGraphicsComponent->getImage()->getDestinationScaleFactor());
			renderer->render(*currentGraphicsComponent, dest);
			currentRenderableIt++;
		}
	}


}


//while (currentRenderableIt != lastRenderableIt) {
//	currentRenderable= *currentRenderableIt;
//	if (currentRenderable->getImage() != NULL) {
//		SubArea dest;
//		dest.x = currentRenderable-> getPos().x -currentRenderable-> getOffset().x;
//		dest.y = currentRenderable-> getPos().y - currentRenderable->getOffset().y;
//		dest.w = (float) currentRenderable-> getSrc().w /(currentRenderable->getImage()->getDestinationScaleFactor()); //TODO uitzoeken hoe src en dest werken en hoe abstractie van SDLRect...
//		dest.h = (float) currentRenderable-> getSrc().h /(currentRenderable->getImage()->getDestinationScaleFactor());
//		renderer->render(*currentRenderable, dest);
//		currentRenderableIt++;
//	}
//}
