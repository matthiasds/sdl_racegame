/*
 * Sprite.cpp
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#include "Sprite.h"
#include "Image.h"
#include "Renderer.h"


/** Sprite:
 *
 * A Sprite is instantiated from a Image instance using
 * Image::createSprite() which will increase the reference counter of
 * the image. Sprite::~Sprite() will release the reference to the
 * source image instance.
 *
 * A Sprite represents an area in a Image, it can be the full image or
 * just parts of the image such as an image spritemap.
 *
 * Sprite constructor is private to prevent creation of Sprites
 * outside of Image instance.
 *
 *
 */

namespace Au_2Drenderer {

Sprite::Sprite(Image *_image , Rect renderArea)
	: renderArea(renderArea)
	, image(_image)
	, src(Rect())
	, dest()
	, active(true) {
	image->ref();
	//create the minimum 1 offset;
	Point zeroOffset = Point(0,0);
	offset.push_back(zeroOffset);
}

Sprite::~Sprite() {
	image->unref();
}

void Sprite::setOffset(const Point& _offset) {
	offset.at(0) = _offset;
}

void Sprite::setOffset(const int x, const int y) {
	offset.at(0).x = x;
	offset.at(0).y = y;
}

//adding another offset to the vector will result in an extra rendering of the image on the new offset
void Sprite::addExtraRenderOffset(const int x, const int y) {
	Point extraOffset = Point(x,y);
	offset.push_back(extraOffset);
}

//this will disable all extra renderings and only keep the first default offset
void Sprite::clearExtraRenderOffsets() {
	offset.resize(1);
}



std::vector<Point>& Sprite::getOffset() {

	return offset;
}

void Sprite::setClip(const Rect& clip) {
	src = clip;
}

void Sprite::setClip(const int x, const int y, const int w, const int h) {
	src.x = x;
	src.y = y;
	src.w = w;
	src.h = h;
}

void Sprite::setClipX(const int x) {
	src.x = x;
}

void Sprite::setClipY(const int y) {
	src.y = y;
}

void Sprite::setClipW(const int w) {
	src.w = w;
}

void Sprite::setClipH(const int h) {
	src.h = h;
}


Rect Sprite::getClip() {
	return src;
}
void Sprite::setDest(const Rect& _dest) {
	dest.x = (float)_dest.x;
	dest.y = (float)_dest.y;
}

void Sprite::setDest(const Point& _dest) {
	dest.x = (float)_dest.x;
	dest.y = (float)_dest.y;
}

void Sprite::setDest(int x, int y) {
	dest.x = (float)x;
	dest.y = (float)y;
}

void Sprite::setDestX(int x) {
	dest.x = (float)x;
}

void Sprite::setDestY(int y) {
	dest.y = (float)y;
}

FPoint Sprite::getDest() {
	return dest;
}

int Sprite::getGraphicsWidth() {
	return image->getWidth();
}

int Sprite::getGraphicsHeight() {
	return image->getHeight();
}

Image * Sprite::getGraphics() {
	return (Image *)image;
}


const Rect& Sprite::getRenderArea() const {
	return renderArea;
}

void Sprite::setRenderArea(const Rect& renderArea) {
	this->renderArea = renderArea;
}

bool Sprite::isActive() const {
	return active;
}

void Sprite::setActive(bool active) {
	this->active = active;
}

const Rect& Sprite::getTextureRenderBox() const {
	return textureRenderBox;
}

void Sprite::setTextureRenderBox(const Rect& textureRenderBox) {
	this->textureRenderBox = textureRenderBox;
}

}
