

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <string>

//#include "SharedResources.h"
#include "Settings.h"
#include "Utils.h"

#include <SDL2\SDL_ttf.h>
#include "SdlHardwareRenderer.h"


SdlHardwareImage::SdlHardwareImage(Renderer *_device, SDL_Renderer *_renderer)
	: Image(_device)
	, renderer(_renderer)
	, surface(NULL) {
}

SdlHardwareImage::~SdlHardwareImage() {
}

int SdlHardwareImage::getWidth() const {
	int w, h;
	SDL_QueryTexture(surface, NULL, NULL, &w, &h);
	return (surface ? w : 0);
}

int SdlHardwareImage::getHeight() const {
	int w, h;
	SDL_QueryTexture(surface, NULL, NULL, &w, &h);
	return (surface ? h : 0);
}

void SdlHardwareImage::fillWithColor(Uint32 color) {
	if (!surface) return;

	Uint32 u_format;
	SDL_QueryTexture(surface, &u_format, NULL, NULL, NULL);
	SDL_PixelFormat* format = SDL_AllocFormat(u_format);

	if (!format) return;

	SDL_Color rgba;
	SDL_GetRGBA(color, format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
	SDL_FreeFormat(format);

	SDL_SetRenderTarget(renderer, surface);
	SDL_SetTextureBlendMode(surface, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g , rgba.b, rgba.a);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
}

/*
 * Set the pixel at (x, y) to the given value
 */
void SdlHardwareImage::drawPixel(int x, int y, Uint32 pixel) {
	if (!surface) return;

	Uint32 u_format;
	SDL_QueryTexture(surface, &u_format, NULL, NULL, NULL);
	SDL_PixelFormat* format = SDL_AllocFormat(u_format);

	if (!format) return;

	SDL_Color rgba;
	SDL_GetRGBA(pixel, format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
	SDL_FreeFormat(format);

	SDL_SetRenderTarget(renderer, surface);
	SDL_SetTextureBlendMode(surface, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
	SDL_RenderDrawPoint(renderer, x, y);
	SDL_SetRenderTarget(renderer, NULL);
}

Uint32 SdlHardwareImage::MapRGB(Uint8 r, Uint8 g, Uint8 b) {
	if (!surface) return 0;

	Uint32 u_format;
	SDL_QueryTexture(surface, &u_format, NULL, NULL, NULL);
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

Uint32 SdlHardwareImage::MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	if (!surface) return 0;

	Uint32 u_format;
	SDL_QueryTexture(surface, &u_format, NULL, NULL, NULL);
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

Image* SdlHardwareImage::resize(int width, int height) {
	if(!surface || width <= 0 || height <= 0)
		return NULL;

	SdlHardwareImage *scaled = new SdlHardwareImage(device, renderer);
	if (!scaled) return NULL;

	scaled->surface = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width, height);

	if (scaled->surface != NULL) {
		// copy the source texture to the new texture, stretching it in the process
		SDL_SetRenderTarget(renderer, scaled->surface);
		SDL_RenderCopyEx(renderer, surface, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
		SDL_SetRenderTarget(renderer, NULL);

		// Remove the old surface
		this->unref();
		return scaled;
	}
	else {
		delete scaled;
	}

	return NULL;
}

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

	if (HWSURFACE) flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_TARGETTEXTURE;

	if (screen != NULL) renderer = SDL_CreateRenderer(screen, -1, flags);

	if (renderer && FULLSCREEN && (window_w != width || window_h != height)) {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(renderer, width, height);
	}

	if (screen != NULL && renderer != NULL) {
		is_initialized = true;
		updateTitleBar();
		return 0;
	}
	else {
		logError("SDLHardwareRenderDevice: createContext() failed: %s", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
}

SubArea SdlHardwareRenderer::getContextSize() {
	SubArea size;
	size.x = size.y = 0;
	SDL_GetWindowSize(screen, &size.w, &size.h);

	return size;
}

int SdlHardwareRenderer::render(Renderable& r, SubArea dest) {
    SDL_Rect src = r.getSrc();
    SDL_Rect _dest = dest;
	return SDL_RenderCopy(renderer, static_cast<SdlHardwareImage *>(r.getImage())->surface, &src, &_dest);
}

int SdlHardwareRenderer::render(Sprite *r) {
	if (r == NULL) {
		return -1;
	}
	if ( !localToGlobal(r) ) {
		return -1;
	}

	// negative x and y clip causes weird stretching
	// adjust for that here
	if (m_clip.x < 0) {
		m_clip.w -= abs(m_clip.x);
		m_dest.x += abs(m_clip.x);
		m_clip.x = 0;
	}
	if (m_clip.y < 0) {
		m_clip.h -= abs(m_clip.y);
		m_dest.y += abs(m_clip.y);
		m_clip.y = 0;
	}

	m_dest.w = m_clip.w;
	m_dest.h = m_clip.h;

    SDL_Rect src = m_clip;
    SDL_Rect dest = m_dest;
	return SDL_RenderCopy(renderer, static_cast<SdlHardwareImage *>(r->getGraphics())->surface, &src, &dest);
}

int SdlHardwareRenderer::renderToImage(Image* src_image, SubArea& src, Image* dest_image, SubArea& dest, bool dest_is_transparent) {
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

int SdlHardwareRenderer::renderText(
	TTF_Font *ttf_font,
	const std::string& text,
	Color color,
	SubArea& dest
) {
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

void SdlHardwareRenderer::drawPixel(
	int x,
	int y,
	Uint32 color
) {
	Uint32 u_format = SDL_GetWindowPixelFormat(screen);
	SDL_PixelFormat* format = SDL_AllocFormat(u_format);

	if (!format) return;

	SDL_Color rgba;
	SDL_GetRGBA(color, format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
	SDL_FreeFormat(format);

	SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
	SDL_RenderDrawPoint(renderer, x, y);
}

void SdlHardwareRenderer::drawLine(
	int x0,
	int y0,
	int x1,
	int y1,
	Uint32 color
) {
	Uint32 u_format = SDL_GetWindowPixelFormat(screen);
	SDL_PixelFormat* format = SDL_AllocFormat(u_format);

	if (!format) return;

	SDL_Color rgba;
	SDL_GetRGBA(color, format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
	SDL_FreeFormat(format);

	SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
	SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}

void SdlHardwareRenderer::drawRectangle(
	const Point& p0,
	const Point& p1,
	Uint32 color
) {
	drawLine(p0.x, p0.y, p1.x, p0.y, color);
	drawLine(p1.x, p0.y, p1.x, p1.y, color);
	drawLine(p0.x, p0.y, p0.x, p1.y, color);
	drawLine(p0.x, p1.y, p1.x, p1.y, color);
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
			logError("SDLHardwareRenderDevice: SDL_CreateTexture failed: %s", SDL_GetError());
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
	/*replacement of 	title = strdup("Game Window") due to c++11 not allowing it */
	std::string title_s = "Game Window";
	title = new char [title_s.size()+1];
	std::copy(title_s.begin(), title_s.end(), title);
	title[title_s.size()] = '\0';


	titlebar_icon = IMG_Load("images/logo/icon.png");

	if (title) SDL_SetWindowTitle(screen, title);
	if (titlebar_icon) SDL_SetWindowIcon(screen, titlebar_icon);
}

void SdlHardwareRenderer::listModes(std::vector<SubArea> &modes) {
	int mode_count = SDL_GetNumDisplayModes(0);

	for (int i=0; i<mode_count; i++) {
		SDL_DisplayMode display_mode;
		SDL_GetDisplayMode(0, i, &display_mode);

		if (display_mode.w == 0 || display_mode.h == 0) continue;

		SubArea mode_rect;
		mode_rect.w = display_mode.w;
		mode_rect.h = display_mode.h;
		modes.push_back(mode_rect);

		if (display_mode.w < MIN_VIEW_W || display_mode.h < MIN_VIEW_H) {
			// make sure the resolution fits in the constraints of MIN_VIEW_W and MIN_VIEW_H
			modes.pop_back();
		}
		else {
			// check previous resolutions for duplicates. If one is found, drop the one we just added
			for (unsigned j=0; j<modes.size()-1; ++j) {
				if (modes[j].w == display_mode.w && modes[j].h == display_mode.h) {
					modes.pop_back();
					break;
				}
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
			logError("SDLHardwareRenderDevice: %s: %s", errormessage.c_str(), IMG_GetError());
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


