

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "Point.h"
#include "Rect.h"
#include "Color.h"

#include <SDL2\SDL_ttf.h>
#include "SdlHardwareRenderer.h"
#include "GameOptions.h"


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
		SDL_SetTextureBlendMode(scaled->surface, SDL_BLENDMODE_BLEND);
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

bool SdlHardwareImage::getAlphaXY(int x, int y)
{
	if (!surface) return 0;
	SDL_SetTextureBlendMode(surface, SDL_BLENDMODE_BLEND);
	/* get the format of the texture */
	Uint32 u_format;
	int width = 1;
	int height= 1;
	int textureAccessType;
	SDL_QueryTexture(surface, &u_format, &textureAccessType, NULL, NULL);

	/*get the pixelformat and corresponding BytesPerPixel representing each pixel of the texture */
	SDL_PixelFormat* format = SDL_AllocFormat(u_format);
	int bytesPerPixel = format->BytesPerPixel;
	int pitch = width * bytesPerPixel;

	/*get the pixels */
	int pixel;
	void *pixels = &pixel;

	/* only read 1 pixel */
	SDL_Rect targetpixels;
	targetpixels.x = x;
	targetpixels.y = y;
	targetpixels.w = width;
	targetpixels.h = height;

	const std::size_t pixelSize = pitch*height;
	if (textureAccessType == SDL_TEXTUREACCESS_TARGET) {
		//pixels = new int; //malloc(pixelSize);//operator new(pixelSize);
		SDL_SetRenderTarget(renderer, surface);
		if( SDL_RenderReadPixels(renderer, &targetpixels, 0, pixels, pitch) != 0 )
		{
			std::cerr << "Unable to read pixels from texture! " << SDL_GetError() << std::endl;
			return false;
		}
	}
	else if (textureAccessType == SDL_TEXTUREACCESS_STREAMING) {
		/* lock the complete texture (NULL), we get back a void pointer (implementation depending on the pixelformat) to the locked pixels and the pitch=length of one row pixels) */
		if( SDL_LockTexture(surface, &targetpixels, &pixels, &pitch) != 0 )
		{
			std::cerr << "Unable to lock texture! " << SDL_GetError() << std::endl;
			return false;
		}
	}
	else {
		std::cerr << "The texture has an unsuuported TEXTUREACCES type! " << std::endl;
		return 0;
	}



	/*convert pixels from void to 32 bit */
	Uint32 *upixels = (Uint32*) pixels;



	/*Get the requested pixel */

	/* Uint8* requestedPixel = (Uint8*) upixels + y * (pitch) + x * bytesPerPixel; //for multiple pixels*/
	 Uint8* requestedPixel = (Uint8*) upixels;

	/*get the color of the pixel*/
	Uint32 pixelColor;

	switch(bytesPerPixel)
	{
		case(1):
			pixelColor = *requestedPixel;
			break;
		case(2):
			pixelColor = *(Uint16*)requestedPixel;
			break;
		case(3):
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				pixelColor = requestedPixel[0] << 16 | requestedPixel[1] << 8 | requestedPixel[2];
			else
				pixelColor = requestedPixel[0] | requestedPixel[1] << 8 | requestedPixel[2] << 16;
			break;
		case(4):
			pixelColor = *(Uint32*)requestedPixel;
			break;
	}

	Uint8 red, green, blue, alpha;
	SDL_GetRGBA(pixelColor, format, &red, &green, &blue, &alpha);

	SDL_UnlockTexture(surface);

//	if (textureAccessType == SDL_TEXTUREACCESS_TARGET) {
//		delete pixels);
//	}

	return alpha>200;
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




