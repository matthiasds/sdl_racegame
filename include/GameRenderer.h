/*
 * GameRenderer.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef GAMERENDERER_H_
#define GAMERENDERER_H_

#include "CommonIncludes.h"
#include "RenderDevice.h"
#include "Entity.h"

class GameRenderer {
private:
	RenderDevice * renderer;
public:
	GameRenderer(RenderDevice * renderer);
	virtual ~GameRenderer();
	//MapRenderer(const MapRenderer &copy); // not implemented

	int load(std::string filename);
	void logic();
	void drawRenderable(std::vector<Renderable>::iterator r_cursor);
	void render(std::vector<Entity> &gameEntities);
	// some events can trigger powers
	void activatePower(int power_index, unsigned statblock_index, FPoint &target);

	bool isValidTile(const unsigned &tile);

	// indicates that the map was changed by an event, so the GameStatePlay
	// will tell the mini map to update.
	bool map_change;

	//MapCollision collider;

	// event-created loot or items
	std::vector<Event_Component> loot;

	// map soundids
	//std::vector<SoundManager::SoundID> sids;

	// npc handling
	int npc_id;

	//void loadMusic();

	/**
	 * The index of the layer, which mixes with the objects on screen. Layers
	 * before that are painted below objects; Layers after are painted on top.
	 */
	unsigned index_objectlayer;
};

#endif /* GAMERENDERER_H_ */
