/*
 * IRenderable.h
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#ifndef IRENDERABLE_H_
#define IRENDERABLE_H_

class Renderer;

class IRenderable {
public:
	virtual ~IRenderable() {};
	virtual void Render(Renderer& renderer) = 0;
};


#endif /* IRENDERABLE_H_ */

