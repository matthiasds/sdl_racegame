 /*
 * RenderComponent.h
 *
 *  Created on: 6-apr.-2015
 *      Author: Matthias
 */

#ifndef RENDERCOMPONENT_H_
#define RENDERCOMPONENT_H_

#include "IComponent.h"
#include "Renderer.h"
#include "Utils.h"

class RenderComponent : public IComponent{
public:
	RenderComponent(Renderer* renderer) {
		onScreen = false;
		this->renderer = renderer;
	}

	std::vector<Sprite*>& getSprites(){
		return sprites;
	}

	void addStaticSpritefromTexture(std::string texturePath, int textureId, Rect textureRenderBox ,Rect renderWindow) {
	 	Image* image=renderer->loadImage(texturePath);
		Sprite* sprite = image->createSprite(true, renderWindow);
		Point offset=Point(0, 0);
		sprite->setTextureRenderBox(textureRenderBox);
		sprite->setOffset(offset);
		if (textureId >= (int) sprites.size()) {
			sprites.resize(textureId);
			sprites.insert(sprites.begin()+textureId, sprite);
		}
		else {
			sprites.at(textureId) = sprite;
		}
	}

	bool isOnScreen() const {
		return onScreen;
	}

	void setOnScreen(bool onScreen) {
		this->onScreen = onScreen;
	}

private:
	std::vector<Sprite*> sprites;
	bool onScreen;
	Renderer* renderer;

};

#endif /* RENDERCOMPONENT_H_ */
