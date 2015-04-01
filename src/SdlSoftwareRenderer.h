/*
Copyright © 2013 Kurt Rinnert
Copyright © 2013 Igor Paliychuk
Copyright © 2014 Henrik Andersson

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

#ifndef SDLSOFTWARERENDERDEVICE_H
#define SDLSOFTWARERENDERDEVICE_H

#include "Renderer.h"

/** Provide rendering device using SDL_BlitSurface backend.
 *
 * Provide an SDL_BlitSurface implementation for renderning a Renderable to
 * the screen.  Simply dispatches rendering to SDL_BlitSurface().
 *
 * As this is for the FLARE engine, the implementation uses the engine's
 * global settings context, which is included by the interface.
 *
 */


/** SDL Image */
class SdlSoftwareImage : public Image {
public:
	SdlSoftwareImage(Renderer *device);
	virtual ~SdlSoftwareImage();
	int getWidth() const;
	int getHeight() const;

	void fillWithColor(Uint32 color);
	void drawPixel(int x, int y, Uint32 color);
	Uint32 MapRGB(Uint8 r, Uint8 g, Uint8 b);
	Uint32 MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	Image* resize(int width, int height);

	SDL_Surface *surface;

private:
	Uint32 readPixel(int x, int y);
};

class SdlSoftwareRenderer : public Renderer {

public:

	SdlSoftwareRenderer();
	int createContext(int width, int height);
	SubArea getContextSize();

	virtual int render(Renderable& r, SubArea dest);
	virtual int render(Sprite* r);
	virtual int renderToImage(Image* src_image, SubArea& src, Image* dest_image, SubArea& dest, bool dest_is_transparent = false);

	int renderText(TTF_Font *ttf_font, const std::string& text, Color color, SubArea& dest);
	Image* renderTextToImage(TTF_Font* ttf_font, const std::string& text, Color color, bool blended = true);
	void drawPixel(int x, int y, Uint32 color);
	void drawRectangle(const Point& p0, const Point& p1, Uint32 color);
	void blankScreen();
	void commitFrame();
	void destroyContext();
	Uint32 MapRGB(Uint8 r, Uint8 g, Uint8 b);
	Uint32 MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	Image *createImage(int width, int height);
	void setGamma(float g);
	void updateTitleBar();
	void listModes(std::vector<SubArea> &modes);
	void freeImage(Image *image);

	Image* loadImage(std::string filename,
					 std::string errormessage = "Couldn't load image",
					 bool IfNotFoundExit = false);
private:
	void drawLine(int x0, int y0, int x1, int y1, Uint32 color);
	void setSDL_RGBA(Uint32 *rmask, Uint32 *gmask, Uint32 *bmask, Uint32 *amask);

	SDL_Surface* screen;
#if SDL_VERSION_ATLEAST(2,0,0)
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
#endif
	SDL_Surface* titlebar_icon;
	char* title;
};

#endif // SDLSOFTWARERENDERDEVICE_H
