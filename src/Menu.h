/*
 * Menu.h
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#ifndef MENU_H_
#define MENU_H_

#include "Renderer.h"
#include "Utils.h"

class Menu : public Renderable{
public:
	Menu();
	Image* getImage() const override;
	FPoint getPos() const override;
	Point	getOffset() const override;
	uint32_t getPrio() const override;
	RenderDevice* getRenderer() const override;
	SubArea getSrc() const override;
	virtual ~Menu();
private:
	RenderDevice* renderer;
protected:
	virtual void setRenderer(RenderDevice* renderer);
	FPoint pos;
	Point offset;
	uint32_t prio;
	Image* background;
	SubArea src;
};

#endif /* MENU_H_ */
