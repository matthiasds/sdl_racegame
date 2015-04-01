/*
 * Image.h
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */


#ifndef IMAGE_H_
#define IMAGE_H_

#include "Utils.h"
#include "Sprite.h"
class Renderer;

/**  image representation
 * An image can only be instantiated, and is owned, by a RenderDevice
 * For a SDL render device this means SDL_Surface or a SDL_Texture, and
 * by OpenGL render device this is a texture.
 *
 * Image uses a reference counter to control when to free the resource, when the
 * last reference is released, the Image is freed using RenderDevice::freeImage().
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

	class Sprite *createSprite(bool clipToSize = true);
	float getDestinationScaleFactor() const;
	void setDestinationScaleFactor(float destinationScaleFactor);

private:
	Image(Renderer *device);
	virtual ~Image();
	friend class SdlSoftwareImage;
	friend class SdlHardwareImage;

private:
	Renderer *device;
	uint32_t ref_counter;
	float destinationScaleFactor;
};

#endif /* IMAGE_H_ */
