/*
 * Image.cpp
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#include "Image.h"
#include "Renderer.h"

/*
 * Image
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
