 /*
 * SdlInfoRenderComponent.h
 *
 *  Created on: 6-apr.-2015
 *      Author: Matthias
 */

#ifndef SDLINFORENDERCOMPONENT_H_
#define SDLINFORENDERCOMPONENT_H_

#include "IComponent.h"
#include "Renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>


class SdlInfoRenderComponent : public IComponent{
public:
	SdlInfoRenderComponent(Renderer* renderer, std::string speedFontPath, int speedFontSize , std::string otherInfoFontPath,int otherInfoFontSize) {
		speedFont = loadFont(speedFontPath, speedFontSize);
		otherInfoFont = loadFont(otherInfoFontPath, otherInfoFontSize );
	}




	TTF_Font* getSpeedFont() {
		return speedFont;
	}

	TTF_Font* getOtherInfoFont() {
		return otherInfoFont;
	}


private:

	TTF_Font*  loadFont(std::string fontPath, int size){
			TTF_Font* font;
			font = TTF_OpenFont( fontPath.c_str(), size );
			if( font == NULL )
			{
				std::cerr << "Failed to load font " << fontPath << ", Error: " << TTF_GetError() << std::endl;
			}
			return font;
	}

	TTF_Font * speedFont;
	TTF_Font * otherInfoFont;

};

#endif /* SDLINFORENDERCOMPONENT_H_ */
