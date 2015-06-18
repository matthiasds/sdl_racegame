
#include <assert.h>
#include <stdio.h>
#include "Renderer.h"
#include "Utils.h"






/*
 * RenderDevice
 */
Renderer::Renderer()
	: is_initialized(false) {
}

Renderer::~Renderer() {
}

void Renderer::destroyContext() {
	if (!cache.empty()) {
		IMAGE_CACHE_CONTAINER_ITER it;
		logError("RenderDevice: Image cache still holding these images:");
		it = cache.begin();
		while (it != cache.end()) {
			logError("%s %d", it->first.c_str(), it->second->getRefCount());
			++it;
		}
	}
	assert(cache.empty());
}

Image * Renderer::cacheLookup(std::string &filename) {
	IMAGE_CACHE_CONTAINER_ITER it;
	it = cache.find(filename);
	if (it != cache.end()) {
		it->second->ref();
		return it->second;
	}
	return NULL;
}

void Renderer::cacheStore(std::string &filename, Image *image) {
	if (image == NULL) return;
	cache[filename] = image;
}

void Renderer::cacheRemove(Image *image) {
	IMAGE_CACHE_CONTAINER_ITER it = cache.begin();
	while (it != cache.end()) {
		if (it->second == image)
			break;
		++it;
	}

	if (it != cache.end()) {
		cache.erase(it);
	}
}

bool Renderer::renderAreaConversion(Sprite *sprite, int offsetIndex) {
	if (!sprite->isActive()) { //check if sprite is not temporary disabled
		return false;
	}
	destClip = sprite->getClip();
	srcClip = sprite->getClip();
	Rect renderArea = sprite->getRenderArea();

	//calculate leftScaled right up and down borders of texture. respecting target dimensions and offset
	int leftScaled = (int) (sprite->getDest().x - sprite->getTextureRenderBox().x) - sprite->getOffset().at(offsetIndex).x; // int(sprite->getDest().x - sprite->getOffset().at(offsetIndex).x * sprite->getScale());
	int rightScaled = leftScaled + sprite->getTextureRenderBox().w;
	int upScaled = (int) (sprite->getDest().y - sprite->getTextureRenderBox().y) - sprite->getOffset().at(offsetIndex).y;// int(sprite->getDest().y - sprite->getOffset().at(offsetIndex).y * sprite->getScale());
	int downScaled = upScaled + sprite->getTextureRenderBox().h;

	// Check whether we need to render, so check if scaled texture falls of the screen.
	// If so, compute the correct clipping.
	if (renderArea.w) { //get current window width
		if (leftScaled > renderArea.w) { //check if the texture is rendered completely out of right renderArea border
			return false;		// if outside border do not render
		}
		if (rightScaled < 0) {		// check if the texture is rendered completely out of left renderArea border
			return false;		// if outside border do not render
		}
		if (leftScaled < 0) {			//check if the texture is only partially rendered on the renderArea
			destClip.x = sprite->getClip().x - leftScaled;   //set clipping x position more to the right of texture : current x position + value falling of the screen
			leftScaled = 0; //limit leftScaled side of texture to window left border
		}
		rightScaled = (rightScaled < renderArea.w ? rightScaled : renderArea.w); // limit rightScaled side of texture to window right border
		destClip.w = ((float) (rightScaled - leftScaled)); // * r->getScale(); // limit rightScaled side of texture to window right border

		//clip the uncscaled src rectangle in the same way
		srcClip.x = (float)sprite->getGraphicsWidth()/sprite->getTextureRenderBox().w * destClip.x;
		srcClip.w = (float)sprite->getGraphicsWidth()/sprite->getTextureRenderBox().w * destClip.w;

	}
	if (renderArea.h) { //get current window height
		if (upScaled > renderArea.h) {  //check if the texture is rendered completely out of upper renderArea border
			return false;		// if outside border do not render
		}
		if (downScaled < 0) {			//check if the texture is rendered completely out of lower renderArea border
			return false;		// if outside border do not render
		}
		if (upScaled < 0) {			//check if the texture is partially rendered out of the bottom border
			destClip.y = sprite->getClip().y - upScaled;   //set clipping y position more to the top of texture :current y position + value falling of the screen
			upScaled = 0; //limit under side of texture to window low border
		}
		downScaled = (downScaled < renderArea.h ? downScaled : renderArea.h); // limit top side of texture to window right border
		destClip.h = ((float)(downScaled - upScaled)); //* r->getScale();

		//clip the uncscaled src rectangel in the same way
		srcClip.y = (float)sprite->getGraphicsHeight()/sprite->getTextureRenderBox().h * destClip.y;
		srcClip.h = (float)sprite->getGraphicsHeight()/sprite->getTextureRenderBox().h * destClip.h;
	}



	m_dest.x = leftScaled + renderArea.x;
	m_dest.y = upScaled + renderArea.y;

	return true;
}
