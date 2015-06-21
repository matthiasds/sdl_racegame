/*
 * Image.cpp
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#include "Image.h"
#include "Renderer.h"

namespace Au_2Drenderer {


/**
 * Image:
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
Image::Image(Renderer *_device)
	: ref_counter(1),
	  device(_device)
	  {
}

Image::~Image() {
	/* free resource allocated by renderdevice */
	device->freeImage(this);
}

void Image::ref() {
	++ref_counter;
}

void Image::unref() {
	--ref_counter;
	if (ref_counter == 0)
		delete this;
}

uint32_t Image::getRefCount() const {
	return ref_counter;
}

int Image::getWidth() const {
	return 0;
}

int Image::getHeight() const {
	return 0;
}


Sprite *Image::createSprite(bool clipToSize, Rect renderArea) {
	Sprite *sprite;
	sprite = new Sprite(this, renderArea);
	if (clipToSize)
		sprite->setClip(0, 0, this->getWidth(), this->getHeight());
	return sprite;
}

Renderer* Image::getRenderer() const {
	return device;
}

}
