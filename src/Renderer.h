
#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <vector>
#include <map>
#include <SDL2\SDL_ttf.h>
#include "Utils.h"
#include "Sprite.h"
#include "Image.h"
#include "Renderable.h"


class Image;
class Renderer;











/** Provide abstract interface for FLARE engine rendering devices.
 *
 * Provide an abstract interface for renderning a Renderable to the screen.
 * Each rendering device implementation must fully implement the interface.
 * The idea is that the render() method replicates the behaviour of
 * SDL_BlitSurface() with different rendering backends, but bases on the
 * information carried in a Renderable struct.
 *
 * As this is for the FLARE engine, implementations use the the engine's global
 * settings context.
 *
 *
 */
class Renderer {

public:
	Renderer();
	virtual ~Renderer();

	/** Context operations */
	virtual int createContext(int width, int height) = 0;
	virtual void destroyContext() = 0;
	virtual SubArea getContextSize() = 0;
	virtual void listModes(std::vector<SubArea> &modes) = 0;
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
	virtual int render(Renderable& r, SubArea dest) = 0;
	virtual int renderToImage(Image* src_image, SubArea& src, Image* dest_image, SubArea& dest,
							  bool dest_is_transparent = false) = 0;
	virtual int renderText(TTF_Font *ttf_font, const std::string& text, Color color, SubArea& dest) = 0;
	virtual Image* renderTextToImage(TTF_Font* ttf_font, const std::string& text, Color color, bool blended = true) = 0;
	virtual void blankScreen() = 0;
	virtual void commitFrame() = 0;
	virtual void drawPixel(int x, int y, Uint32 color) = 0;
	virtual void drawRectangle(const Point& p0, const Point& p1, Uint32 color) = 0;
	virtual Uint32 MapRGB(Uint8 r, Uint8 g, Uint8 b) = 0;
	virtual Uint32 MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) = 0;

protected:
	/* Compute clipping and global position from local frame. */
	bool localToGlobal(Sprite *r);

	/* Image cache operations */
	Image *cacheLookup(std::string &filename);
	void cacheStore(std::string &filename, Image *);
	void cacheRemove(Image *image);

	bool is_initialized;

	SubArea m_clip;
	SubArea m_dest;

private:
	typedef std::map<std::string, Image *> IMAGE_CACHE_CONTAINER;
	typedef IMAGE_CACHE_CONTAINER::iterator IMAGE_CACHE_CONTAINER_ITER;

	IMAGE_CACHE_CONTAINER cache;

	virtual void drawLine(int x0, int y0, int x1, int y1, Uint32 color) = 0;
};

#endif // RENDERDEVICE_H
