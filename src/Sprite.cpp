/*
 * Sprite.cpp
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#include "Sprite.h"
#include "Image.h"


Sprite::Sprite(Image *_image)
	: local_frame(SubArea())
	, image(_image)
	, src(SubArea())
	, offset()
	, dest() {
	image->ref();
}

Sprite::~Sprite() {
	image->unref();
}

void Sprite::setOffset(const Point& _offset) {
	this->offset = _offset;
}

void Sprite::setOffset(const int x, const int y) {
	this->offset.x = x;
	this->offset.y = y;
}

Point Sprite::getOffset() {

	return offset;
}

void Sprite::setClip(const SubArea& clip) {
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


SubArea Sprite::getClip() {
	return src;
}
void Sprite::setDest(const SubArea& _dest) {
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

