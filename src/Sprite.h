/*
 * Sprite.h
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include <vector>
#include "Point.h"
#include "Rect.h"


namespace Au_2Drenderer {

class Image;

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

class Sprite {

public:
	virtual ~Sprite();



	virtual Image * getGraphics();
	//virtual void setOffset();
	virtual void setOffset(const Point& _offset);
	virtual void setOffset(const int x, const int y);
	virtual std::vector<Point>& getOffset();
	void addExtraRenderOffset(const int x, const int y);
	void clearExtraRenderOffsets();
	virtual void setClip(const Rect& clip);
	virtual void setClip(const int x, const int y, const int w, const int h);
	virtual void setClipX(const int x);
	virtual void setClipY(const int y);
	virtual void setClipW(const int w);
	virtual void setClipH(const int h);
	virtual Rect getClip();
	virtual void setDest(const Rect& _dest);
	virtual void setDest(const Point& _dest);
	virtual void setDest(int x, int y);
	virtual void setDestX(int x);
	virtual void setDestY(int y);
	virtual FPoint getDest();
	virtual int getGraphicsWidth();
	virtual int getGraphicsHeight();
	const Rect& getRenderArea() const;
	void setRenderArea(const Rect& renderArea);
	bool isActive() const;
	void setActive(bool active);
	const Rect& getTextureRenderBox() const;
	void setTextureRenderBox(const Rect& textureRenderBox);

private:
	Sprite(Image *,  Rect renderArea);
	friend class Image;

protected:
	Rect renderArea; //subwindow whereto the sprite is rendered (cliping is based on this subwindow)
	/** reference to source image */
	Image *image;
	Rect src; // location on the sprite in pixel coordinates.
	//sprite has support for multiple offsets to enable rendering a texture multiple times at once on different postion.

	//give vector multiple offsets to be able to render the sprite multiple times for backgrounds or particles
	std::vector<Point> offset;      // offset from map_pos to topleft corner of sprite
	FPoint dest;
	bool active;  //boolean to temporary disable rendering
	Rect textureRenderBox;
};

}


#endif /* SPRITE_H_ */
