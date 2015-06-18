

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

	void fillWithColor(Uint32 color) override;
	void drawPixel(int x, int y, Uint32 color) override;
	Uint32 MapRGB(Uint8 r, Uint8 g, Uint8 b) override;
	Uint32 MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) override;
	Image* resize(int width, int height) override;
	bool getAlphaXY(int x, int y) override;

	SDL_Renderer *renderer;
	SDL_Texture *surface;
};

class SdlHardwareRenderer : public Renderer {

public:

	SdlHardwareRenderer();
	int createContext(int width, int height);
	Rect getContextSize();

	virtual int render(Image *image, Rect src, Rect dest);
	virtual int render(Sprite* r);
	virtual int renderToImage(Image* src_image, Rect& src, Image* dest_image, Rect& dest, bool dest_is_transparent = false);

	int renderText(TTF_Font *ttf_font, const std::string& text, Color color, Rect& dest) override;
	Image *renderTextToImage(TTF_Font* ttf_font, const std::string& text, Color color, bool blended = true) override;
	void drawPixel(int x, int y, Color color) override;
	void drawRectangle(const Point& p0, const Point& p1, Color color) override;
	void drawFilledRectangle(const Point& p0, const Point& p1, Color color) override;
	void blankScreen();
	void commitFrame();
	void destroyContext();
	Uint32 MapRGB(Uint8 r, Uint8 g, Uint8 b);
	Uint32 MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	Image *createImage(int width, int height);
	void setGamma(float g);
	void updateTitleBar();
	void listModes(std::vector<Rect> &modes);
	void freeImage(Image *image);
	void drawLine(int x0, int y0, int x1, int y1, Color color) override;

	Image* loadImage(std::string filename,
					 std::string errormessage = "Couldn't load image",
					 bool IfNotFoundExit = false);
private:


	SDL_Window *screen;
	SDL_Renderer *renderer;
	SDL_Surface* titlebar_icon;
	char* title;
};


#endif // SDLHARDWARERENDERDEVICE_H
