/*
 * BackgroundTile.h
 *
 *  Created on: 17-apr.-2015
 *      Author: Matthias
 */

#ifndef BACKGROUNDTILE_H_
#define BACKGROUNDTILE_H_

#include "Point.h"


enum TileType { ROAD_3_LANES, GRASS_FULL };

class BackgroundTile {
public:
	BackgroundTile(int textureId, FPoint position, TileType tileType);

	virtual ~BackgroundTile();

	FPoint& getPosition();

	int getTextureId();

	TileType getType() const;

private:
	int textureId;
	FPoint position;
	TileType type;
};

#endif /* BACKGROUNDTILE_H_ */
