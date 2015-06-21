

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "Point.h"
#include "Rect.h"
#include "Color.h"

#include <SDL2\SDL_ttf.h>
#include "SdlHardwareRenderer.h"
#include "SdlHardwareImage.h"
#include "GameOptions.h"

/**
 * SdlHardwareRenderer
 * Here all rendering functions for the SDL Hardware renderer (needs SDL 2) are implemented
 *
 * functions are based on http://lazyfoo.net/ SDL Tutorials and some other open examples
 */

using namespace Au_2Drenderer;

namespace Au_sdl {

SdlHardwareRenderer::SdlHardwareRenderer()
: screen(NULL)
, renderer(NULL)
, titlebar_icon(NULL)
, title(NULL)
{
	std::cout << "Using Render Device: SDLHardwareRenderDevice (hardware, SDL 2)" << std::endl;
}

int SdlHardwareRenderer::createContext(int width, int height) {
	int window_w = width;
	int window_h = height;

	if (FULLSCREEN) {
		// make the window the same size as the desktop resolution
		SDL_DisplayMode desktop;
		if (SDL_GetDesktopDisplayMode(0, &desktop) == 0) {
			window_w = desktop.w;
			window_h = desktop.h;
		}
	}

	Uint32 flags = 0;

	if (FULLSCREEN) flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	else flags = SDL_WINDOW_SHOWN;

	if (is_initialized) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(screen);
		renderer = NULL;
		screen = NULL;
	}

	screen = SDL_CreateWindow("GameWindow",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			window_w, window_h,
			flags);

	flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

	if (screen != NULL) renderer = SDL_CreateRenderer(screen, -1, flags);

	if (renderer && FULLSCREEN && (window_w != width || window_h != height)) {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(renderer, width, height);
	}

	//Initialize SDL_ttf
	if( TTF_Init() == -1 )
	{
		std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: "<< TTF_GetError() << std::endl;
	}

	if (screen != NULL && renderer != NULL) {
		is_initialized = true;
		updateTitleBar();
		return 0;
	}
	else {
		std::cerr << "SDLHardwareRenderDevice: createContext() failed: %s" << SDL_GetError();
		SDL_Quit();
		exit(1);
	}
}

Rect SdlHardwareRenderer::getContextSize() {
	Rect size;
	size.x = size.y = 0;
	SDL_GetWindowSize(screen, &size.w, &size.h);

	return size;
}

int SdlHardwareRenderer::render(Image *image, Rect src, Rect dest) {
	SDL_Rect _src = src;
	SDL_Rect _dest = dest;
	return SDL_RenderCopy(renderer, static_cast<SdlHardwareImage *>(image)->surface, &_src, &_dest);
}

int SdlHardwareRenderer::render(Sprite *sprite) {
	if (sprite == NULL) {
		return -1;
	}
	int returnVal;
	for (int offsetIndex = 0;  offsetIndex < (int) sprite->getOffset().size(); offsetIndex++) {
		returnVal = !renderAreaConversion(sprite, offsetIndex );
		if ( returnVal == 0) { //rendering of sprite at offset inside area
			// negative x and y clip causes weird stretching
			// adjust for that here
			if (destClip.x < 0) {
				destClip.w -= abs(destClip.x);
				m_dest.x += abs(destClip.x);
				destClip.x = 0;
			}
			if (destClip.y < 0) {
				destClip.h -= abs(destClip.y);
				m_dest.y += abs(destClip.y);
				destClip.y = 0;
			}

			m_dest.w = destClip.w;
			m_dest.h = destClip.h;

			SDL_Rect src = srcClip;
			SDL_Rect dest = m_dest;
			returnVal =  SDL_RenderCopy(renderer, static_cast<SdlHardwareImage *>(sprite->getGraphics())->surface, &src, &dest);
			if (returnVal != 0) {
				std::cerr << "Unable to render texture! " << SDL_GetError() << std::endl;
			}
		}

	}

	return returnVal;
}



int SdlHardwareRenderer::renderToImage(Image* src_image, Rect& src, Image* dest_image, Rect& dest, bool dest_is_transparent) {
	if (!src_image || !dest_image)
		return -1;

	if (SDL_SetRenderTarget(renderer, static_cast<SdlHardwareImage *>(dest_image)->surface) != 0)
		return -1;

	if (dest_is_transparent) {
		// do nothing
		// this block is here to suppress an unused variable compiler warning
	}

	dest.w = src.w;
	dest.h = src.h;
	SDL_Rect _src = src;
	SDL_Rect _dest = dest;

	SDL_SetTextureBlendMode(static_cast<SdlHardwareImage *>(dest_image)->surface, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer, static_cast<SdlHardwareImage *>(src_image)->surface, &_src, &_dest);
	SDL_SetRenderTarget(renderer, NULL);
	return 0;
}

int SdlHardwareRenderer::renderText(TTF_Font *ttf_font, const std::string& text, Color color, Rect& dest) {
	int ret = 0;
	SDL_Texture *surface = NULL;

	SDL_Surface *cleanup = TTF_RenderUTF8_Blended(ttf_font, text.c_str(), color);
	if (cleanup) {
		surface = SDL_CreateTextureFromSurface(renderer,cleanup);
		SDL_FreeSurface(cleanup);
	}

	if (surface == NULL)
		return -1;

	SDL_Rect clip;
	int w, h;
	SDL_QueryTexture(surface, NULL, NULL, &w, &h);

	clip.x = clip.y = 0;
	clip.w = w;
	clip.h = h;

	dest.w = clip.w;
	dest.h = clip.h;
	SDL_Rect _dest = dest;

	ret = SDL_RenderCopy(renderer, surface, &clip, &_dest);

	SDL_DestroyTexture(surface);

	return ret;
}

Image * SdlHardwareRenderer::renderTextToImage(TTF_Font* ttf_font, const std::string& text, Color color, bool blended) {
	SdlHardwareImage *image = new SdlHardwareImage(this, renderer);

	SDL_Surface *cleanup;

	if (blended) {
		cleanup = TTF_RenderUTF8_Blended(ttf_font, text.c_str(), color);
	}
	else {
		cleanup = TTF_RenderUTF8_Solid(ttf_font, text.c_str(), color);
	}

	if (cleanup) {
		image->surface = SDL_CreateTextureFromSurface(renderer, cleanup);
		SDL_FreeSurface(cleanup);
		return image;
	}

	delete image;
	return NULL;
}

void SdlHardwareRenderer::drawPixel(int x, int y, Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(renderer, x, y);
}

void SdlHardwareRenderer::drawLine(int x0, int y0, int x1, int y1, Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}

void SdlHardwareRenderer::drawRectangle(const Point& p0, const Point& p1, Color color) {
	drawLine(p0.x, p0.y, p1.x, p0.y, color);
	drawLine(p1.x, p0.y, p1.x, p1.y, color);
	drawLine(p0.x, p0.y, p0.x, p1.y, color);
	drawLine(p0.x, p1.y, p1.x, p1.y, color);
}

void SdlHardwareRenderer::drawFilledRectangle(const Point& p0, const Point& p1,Color color) {
	SDL_Rect fillRect = { p0.x, p0.y, p1.x - p0.x, p1.y - p0.y };

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect( renderer, &fillRect );

}

void SdlHardwareRenderer::blankScreen() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	return;
}

void SdlHardwareRenderer::commitFrame() {
	SDL_RenderPresent(renderer);
	return;
}

void SdlHardwareRenderer::destroyContext() {
	SDL_FreeSurface(titlebar_icon);
	titlebar_icon = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(screen);
	screen = NULL;

	if (title) {
		free(title);
		title = NULL;
	}

	return;
}

Uint32 SdlHardwareRenderer::MapRGB(Uint8 r, Uint8 g, Uint8 b) {
	Uint32 u_format = SDL_GetWindowPixelFormat(screen);
	SDL_PixelFormat* format = SDL_AllocFormat(u_format);

	if (format) {
		Uint32 ret = SDL_MapRGB(format, r, g, b);
		SDL_FreeFormat(format);
		return ret;
	}
	else {
		return 0;
	}
}

Uint32 SdlHardwareRenderer::MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	Uint32 u_format = SDL_GetWindowPixelFormat(screen);
	SDL_PixelFormat* format = SDL_AllocFormat(u_format);

	if (format) {
		Uint32 ret = SDL_MapRGBA(format, r, g, b, a);
		SDL_FreeFormat(format);
		return ret;
	}
	else {
		return 0;
	}
}

/**
 * create blank surface
 */
Image *SdlHardwareRenderer::createImage(int width, int height) {

	SdlHardwareImage *image = new SdlHardwareImage(this, renderer);

	if (width > 0 && height > 0) {
		image->surface = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width, height);
		if(image->surface == NULL) {
			std::cerr << "SDLHardwareRenderDevice: SDL_CreateTexture failed: %s" << SDL_GetError();
		}
		else {
			SDL_SetRenderTarget(renderer, image->surface);
			SDL_SetTextureBlendMode(image->surface, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 0,0,0,0);
			SDL_RenderClear(renderer);
			SDL_SetRenderTarget(renderer, NULL);
		}
	}

	return image;
}

void SdlHardwareRenderer::setGamma(float g) {
	Uint16 ramp[256];
	SDL_CalculateGammaRamp(g, ramp);
	SDL_SetWindowGammaRamp(screen, ramp, ramp, ramp);
}

void SdlHardwareRenderer::updateTitleBar() {
	if (title) free(title);
	title = NULL;
	if (titlebar_icon) SDL_FreeSurface(titlebar_icon);
	titlebar_icon = NULL;

	if (!screen) return;
	std::string title_s = "MDS Racer";
	title = new char [title_s.size()+1];
	std::copy(title_s.begin(), title_s.end(), title);
	title[title_s.size()] = '\0';


	titlebar_icon = IMG_Load("images/logo/icon.png");

	if (title) SDL_SetWindowTitle(screen, title);
	if (titlebar_icon) SDL_SetWindowIcon(screen, titlebar_icon);
}

void SdlHardwareRenderer::listModes(std::vector<Rect> &modes) {
	int mode_count = SDL_GetNumDisplayModes(0);

	for (int i=0; i<mode_count; i++) {
		SDL_DisplayMode display_mode;
		SDL_GetDisplayMode(0, i, &display_mode);

		if (display_mode.w == 0 || display_mode.h == 0) continue;

		Rect mode_rect;
		mode_rect.w = display_mode.w;
		mode_rect.h = display_mode.h;
		modes.push_back(mode_rect);


		// check previous resolutions for duplicates. If one is found, drop the one we just added
		for (unsigned j=0; j<modes.size()-1; ++j) {
			if (modes[j].w == display_mode.w && modes[j].h == display_mode.h) {
				modes.pop_back();
				break;
			}
		}
	}
}

Image *SdlHardwareRenderer::loadImage(std::string filename, std::string errormessage, bool IfNotFoundExit) {
	// lookup image in cache
	Image *img;
	img = cacheLookup(filename);
	if (img != NULL) return img;

	// load image
	SdlHardwareImage *image = new SdlHardwareImage(this, renderer);
	if (!image) return NULL;

	image->surface = IMG_LoadTexture(renderer, filename.c_str());

	if(image->surface == NULL) {
		delete image;
		if (!errormessage.empty())
			std::cerr << "SDLHardwareRenderDevice: " << errormessage.c_str() << ":" << IMG_GetError();
		if (IfNotFoundExit) {
			SDL_Quit();
			exit(1);
		}
		return NULL;
	}

	// store image to cache
	cacheStore(filename, image);
	return image;
}

void SdlHardwareRenderer::freeImage(Image *image) {
	if (!image) return;

	cacheRemove(image);

	if (static_cast<SdlHardwareImage *>(image)->surface) {
		SDL_DestroyTexture(static_cast<SdlHardwareImage *>(image)->surface);
	}
}

}




