/*
 * GameRenderer.cpp
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#include "GameRenderer.h"
#include "RenderDevice.h"
#include <vector>
#include <algorithm>

GameRenderer::GameRenderer(RenderDevice * renderer) {
	this->renderer=renderer;
	// TODO Auto-generated constructor stub

}

GameRenderer::~GameRenderer() {
	// TODO Auto-generated destructor stub
}

void GameRenderer::drawRenderable(std::vector<Renderable>::iterator r_cursor) {
	if (r_cursor->image != NULL) {
		Rect dest;
		Point p = Point(10,10);
		dest.x = p.x;
		dest.y = p.y;
		renderer->render(*r_cursor, dest);
	}
}

bool priocompare(const Entity &r1, const Entity &r2) {
	return r1.getEntityRender()->prio < r2.getEntityRender()->prio;
}

void GameRenderer::render(std::vector<Entity> &gameEntities) {
	std::sort(gameEntities.begin() , gameEntities.end(), priocompare);


	std::vector<Entity>::iterator r_cursor = gameEntities.begin();
	std::vector<Entity>::iterator r_end = gameEntities.end();

	while (r_cursor != r_end) {
		Rect dest;
		dest.x = r_cursor->getEntityRender()->offset.x;
		dest.y = r_cursor->getEntityRender()->offset.y;
		renderer->render(*(r_cursor->getEntityRender()), dest);
		r_cursor++;
	}

}
