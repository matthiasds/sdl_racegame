/*
 * BackgroundTile.cpp
 *
 *  Created on: 17-apr.-2015
 *      Author: Matthias
 */

#include "BackgroundTile.h"



BackgroundTile::BackgroundTile(int textureId, FPoint position, TileType tileType) {
	this->textureId = textureId;
	this-> position = position;
	this-> type = tileType;
}

BackgroundTile::~BackgroundTile() {
	// TODO Auto-generated destructor stub
}

FPoint& BackgroundTile::getPosition(){
	return position;
}


int BackgroundTile::getTextureId() {
	return textureId;
}

TileType BackgroundTile::getType() const {
	return type;
}


