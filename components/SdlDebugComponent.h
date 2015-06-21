/*
 * SdlDebugComponent.h
 *
 *  Created on: 10-may.-2015
 *      Author: Matthias
 */

#ifndef SDLDEBUGCOMPONENT_H_
#define SDLDEBUGCOMPONENT_H_

#include <iostream>

class SdlDebugComponent : public IComponent {
public:


	SdlDebugComponent(Renderer* renderer, std::string fontPath, int fontSize) {
		size = fontSize;
		color = Color(0,0,255);
		this->fontPath=fontPath;
		loadFont();
		this->renderer = renderer;
	}

	void loadFont()
	{
		font = TTF_OpenFont( fontPath.c_str(), size );
		 if( font == NULL )
			{
				std::cerr << "Failed to load font " << fontPath << ", Error: " << TTF_GetError() << std::endl;
			}
	}


	void setSize(int size) {
			this->size = size;
			loadFont();
		}

	TTF_Font* getFont() {
		return font;
	}

	void setColor(uint8_t r, uint8_t g, uint8_t b ) {
		color = Color(r, g, b);
	}

	Color getColor() {
		return color;
	}


	virtual ~SdlDebugComponent() {
	}

	Renderer* getRenderer() {
		return renderer;
	}

private:
	std::vector<Sprite*> sprites;
	TTF_Font * font;
	Color color;

	int size;
	std::string fontPath;
	Renderer* renderer;
};

#endif /* SDLDEBUGCOMPONENT_H_ */


