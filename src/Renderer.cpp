
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

bool Renderer::localToGlobal(Sprite *r) {
	m_clip = r->getClip();

	int left = int(r->getDest().x - r->getOffset().x);
	int right = left + r->getClip().w;
	int up = int(r->getDest().y - r->getOffset().y);
	int down = up + r->getClip().h;

	// Check whether we need to render.
	// If so, compute the correct clipping.
	if (r->local_frame.w) {
		if (left > r->local_frame.w) {
			return false;
		}
		if (right < 0) {
			return false;
		}
		if (left < 0) {
			m_clip.x = r->getClip().x - left;
			left = 0;
		};
		right = (right < r->local_frame.w ? right : r->local_frame.w);
		m_clip.w = right - left;
	}
	if (r->local_frame.h) {
		if (up > r->local_frame.h) {
			return false;
		}
		if (down < 0) {
			return false;
		}
		if (up < 0) {
			m_clip.y = r->getClip().y - up;
			up = 0;
		};
		down = (down < r->local_frame.h ? down : r->local_frame.h);
		m_clip.h = down - up;
	}

	m_dest.x = left + r->local_frame.x;
	m_dest.y = up + r->local_frame.y;

	return true;
}
