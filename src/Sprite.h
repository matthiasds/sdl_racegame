/*
 * Sprite.h
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include "Utils.h"

class Image;

/** A Sprite representation
 *
 * A Sprite is instantiated from a Image instance using
 * Image::createSprite() which will increase the reference counter of
 * the image. Sprite::~Sprite() will release the reference to the
 * source image instance.
 *
 * A Sprite represents an area in a Image, it can be the full image or
 * just parts of the image such as an image atlas / spritemap.
 *
 * Sprite constructor is private to prevent creation of Sprites
 * outside of Image instance.
 *
 *
 */
class Sprite {

public:
	virtual ~Sprite();

	SubArea local_frame;

	virtual Image * getGraphics();
	//virtual void setOffset();
	virtual void setOffset(const Point& _offset);
	virtual void setOffset(const int x, const int y);
	virtual Point getOffset();
	virtual void setClip(const SubArea& clip);
	virtual void setClip(const int x, const int y, const int w, const int h);
	virtual void setClipX(const int x);
	virtual void setClipY(const int y);
	virtual void setClipW(const int w);
	virtual void setClipH(const int h);
	virtual SubArea getClip();
	virtual void setDest(const SubArea& _dest);
	virtual void setDest(const Point& _dest);
	virtual void setDest(int x, int y);
	virtual void setDestX(int x);
	virtual void setDestY(int y);
	virtual FPoint getDest();
	virtual int getGraphicsWidth();
	virtual int getGraphicsHeight();
private:
	Sprite(Image *);
	friend class Image;

protected:
	/** reference to source image */
	Image *image;
	SubArea src; // location on the sprite in pixel coordinates.
	Point offset;      // offset from map_pos to topleft corner of sprite
	FPoint dest;
};


#endif /* SPRITE_H_ */
