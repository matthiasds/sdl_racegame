/*
 * BackgroundTile.cpp
 *
 *  Created on: 17-apr.-2015
 *      Author: Matthias
 */

#include "BackgroundTile.h"

/**
 * BackgroundTile:
 * This class is used for grouping, setting and retrieving BackgroundTile properties, like ID position, type
 */

/**
 *
 * @param textureId: ID linking the tile with a texture in the renderer
 * @param position: FPoint type, X and Y coordinates of the tile
 * @param tileType: tileType, can be used to extract properties from tiles and make it only possible to make certain compatible tiletypes to follow others.
 * 					Ex. A 3_LANE_TILE can not directly be followed  by a 4_WAY_TILE only through a 3_to_4_WAY_SWITCHER_TILE giving a smooth transition
 */
BackgroundTile::BackgroundTile(int textureId, FPoint position, TileType tileType) {
	this->textureId = textureId;
	this-> position = position;
	this-> type = tileType;
}

/**
 * Destructor: No memory allocated in this class to delete
 */
BackgroundTile::~BackgroundTile() {

}

/**
 * Position getter
 * @return position reference
 */
FPoint& BackgroundTile::getPosition(){
	return position;
}

/**
 * textureId getter
 * @return textureId
 */
int BackgroundTile::getTextureId() {
	return textureId;
}

/**
 * TileType getter
 * @return type
 */
TileType BackgroundTile::getType() const {
	return type;
}


