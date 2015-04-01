

#pragma once
#ifndef SDLHARDWARERENDERDEVICE_H
#define SDLHARDWARERENDERDEVICE_H


#include "Renderer.h"

/** Provide rendering device using SDL_BlitSurface backend.
 *
 * Provide an SDL_BlitSurface implementation for renderning a Renderable to
 * the screen.  Simply dispatches rendering to SDL_BlitSurface().
 *
 * As this is for the FLARE engine, the implementation uses the engine's
 * global settings context, which is included by the interface.
 *
 *
 */

#define SDLKey SDL_Keycode

#define SDL_JoystickName SDL_JoystickNameForIndex


/** SDL Image */
class SdlHardwareImage : public Image {
public:
	SdlHardwareImage(Renderer *device, SDL_Renderer *_renderer);
	virtual ~SdlHardwareImage();
	int getWidth() const;
	int getHeight() const;

	void fillWithColor(Uint32 color);
	void drawPixel(int x, int y, Uint32 color);
	Uint32 MapRGB(Uint8 r, Uint8 g, Uint8 b);
	Uint32 MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	Image* resize(int width, int height);

	SDL_Renderer *renderer;
	SDL_Texture *surface;
};

class SdlHardwareRenderer : public Renderer {

public:

	SdlHardwareRenderer();
	int createContext(int width, int height);
	SubArea getContextSize();

	virtual int render(Renderable& r, SubArea dest);
	virtual int render(Sprite* r);
	virtual int renderToImage(Image* src_image, SubArea& src, Image* dest_image, SubArea& dest, bool dest_is_transparent = false);

	int renderText(TTF_Font *ttf_font, const std::string& text, Color color, SubArea& dest);
	Image *renderTextToImage(TTF_Font* ttf_font, const std::string& text, Color color, bool blended = true);
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

	SDL_Window *screen;
	SDL_Renderer *renderer;
	SDL_Surface* titlebar_icon;
	char* title;
};


#endif // SDLHARDWARERENDERDEVICE_H
