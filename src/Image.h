/*
 * Image.h
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */


#ifndef IMAGE_H_
#define IMAGE_H_

#include "Sprite.h"

namespace Au_2Drenderer {

class Renderer;

/**  image representation
 * an image is an abstract interface for more specific implementations of it.
 *
 * An image can only be instantiated, and is owned, by a RenderDevice
 * For an SDL render device this means an SDL_Surface (software renderer) or a
 * SDL_Texture (hardware renderer), and
 * but appar from SDL this implementation could also be an OpenGL render device with
 * a texture.
 *
 * This Image class has functionallity to reuse the same image when it is used multiple times
 * To be able to destruct the image in a right way a reference counter is used to control
 * when to free the resource, when the last reference is released, the Image is
 * freed using RenderDevice::freeImage().
 *
 * The caller who instantiates an Image is responsible for release the reference
 * to the image when not used anymore.
 *
 * Image is a source for a Sprite and is therefore responsible for instantiating
 * Sprites using Image::createSprite().
 *
 * Creating a Sprite of a Image increases the reference counter, destructor of a
 * Sprite will release the reference to the image.
 *
 */
class Image {
public:
	void ref();
	void unref();
	uint32_t getRefCount() const;

	virtual int getWidth() const;
	virtual int getHeight() const;

	virtual void fillWithColor(Uint32 color) = 0;
	virtual void drawPixel(int x, int y, Uint32 color) = 0;
	virtual Uint32 MapRGB(Uint8 r, Uint8 g, Uint8 b) = 0;
	virtual Uint32 MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) = 0;
	virtual Image* resize(int width, int height) = 0;
	virtual bool getAlphaXY(int x, int y) = 0;


	class Sprite *createSprite(bool clipToSize, Rect renderArea);
	Renderer* getRenderer() const;

	Image(Renderer *device);
	Renderer *device;
	virtual ~Image();

private:
	friend class SdlHardwareImage;
	uint32_t ref_counter;
};

}

#endif /* IMAGE_H_ */
