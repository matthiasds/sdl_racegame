/*
 * Renderable.h
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "Sprite.h"
#include "Image.h"

class Renderable {
public:
	virtual Image* getImage() const = 0;
	virtual FPoint getPos() const = 0;
	virtual Point	getOffset() const = 0;
	virtual uint32_t getPrio() const = 0;
	virtual SubArea getSrc() const = 0;
	virtual void setRenderer(Renderer* renderer) = 0;
	virtual Renderer* getRenderer() const  = 0;
	virtual void addRenderables(std::vector<Renderable> &renderList) = 0;
	virtual ~Renderable() {};
};


#endif /* RENDERABLE_H_ */
