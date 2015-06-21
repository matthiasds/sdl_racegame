
#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <vector>
#include <map>
#include <SDL2\SDL_ttf.h>
#include "Sprite.h"
#include "Image.h"
#include "Point.h"
#include "Rect.h"
#include "Color.h"

namespace Au_2Drenderer {
class Image;


/**
 * Renderer provides an interface for renderning a Renderable to the screen.
 * Each rendering device implementation must implement this interface completely
 * In this way a 2nd level of abstraction for 2D rendering is created.
 *
 * In this way a render uses this standard interface and implements it in his own way
 * an implementation of this is the SdlHardwareRenderer, but it can also be an SdlSoftwareRender
 * or openGLrenderer... The interface is created to be able to work with any general 2D renderer
 *
 */

class Renderer {

public:
	Renderer();
	virtual ~Renderer();

	/** Context operations */
	virtual int createContext(int width, int height) = 0;
	virtual void destroyContext() = 0;
	virtual Rect getContextSize() = 0;
	virtual void listModes(std::vector<Rect> &modes) = 0;
	virtual void setGamma(float g) = 0;
	virtual void updateTitleBar() = 0;

	/** factory functions for Image */
	virtual Image *loadImage(std::string filename,
							 std::string errormessage = "Couldn't load image",
							 bool IfNotFoundExit = false) = 0;
	virtual Image *createImage(int width, int height) = 0;
	virtual void freeImage(Image *image) = 0;

	/** Screen operations */
	virtual int render(Sprite* r) = 0;
	virtual int render(Image *image, Rect src, Rect dest) = 0;
	virtual int renderToImage(Image* src_image, Rect& src, Image* dest_image, Rect& dest,
							  bool dest_is_transparent = false) = 0;
	virtual int renderText(TTF_Font *ttf_font, const std::string& text, Color color, Rect& dest) = 0;
	virtual Image* renderTextToImage(TTF_Font* ttf_font, const std::string& text, Color color, bool blended = true) = 0;
	virtual void blankScreen() = 0;
	virtual void commitFrame() = 0;
	virtual void drawPixel(int x, int y, Color color) = 0;
	virtual void drawRectangle(const Point& p0, const Point& p1, Color color) = 0;
	virtual void drawFilledRectangle(const Point& p0, const Point& p1, Color color) = 0;
	virtual Uint32 MapRGB(Uint8 r, Uint8 g, Uint8 b) = 0;
	virtual Uint32 MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) = 0;
	virtual void drawLine(int x0, int y0, int x1, int y1, Color color) = 0;

protected:
	/* Compute clipping and global position from local render area (part of window). */
	bool renderAreaConversion(Sprite *sprite, int offsetIndex);

	/* Image cache operations */
	Image *cacheLookup(std::string &filename);
	void cacheStore(std::string &filename, Image *);
	void cacheRemove(Image *image);

	bool is_initialized;

	Rect destClip;
	Rect srcClip;
	Rect m_dest;

private:
	typedef std::map<std::string, Image *> IMAGE_CACHE_CONTAINER;
	typedef IMAGE_CACHE_CONTAINER::iterator IMAGE_CACHE_CONTAINER_ITER;

	IMAGE_CACHE_CONTAINER cache;


};
}

#endif // RENDERDEVICE_H
