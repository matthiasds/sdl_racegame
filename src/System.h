/*
 * System.h
 *
 *  Created on: 7-apr.-2015
 *      Author: Matthias
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "BitSize.h"
#include "ComponentTypeManager.h"
#include <vector>
#include "../components/BackgroundRenderComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/PositionComponent.h"
#include "ComponentMapper.h"

class Game;
class Entity;

//abstract class system
class System {
public:
	System();
	virtual void init() = 0;
	//virtual void stop() = 0;
	void setGame(Game* game);
	virtual void processEntity(Entity* entity) = 0;
	virtual ~System();

	template<typename component_type>
	void addComponentType() {
	      //Add Bits to typeflags
		  std::bitset<BITSIZE> componentType = ComponentTypeManager::getBit<component_type>();;
	      typeFlags |= componentType;
	}

	std::bitset<BITSIZE> getTypeFlags();
	void setSystemBit(std::bitset<BITSIZE> bit);
	std::bitset<BITSIZE> getSystemBit();
	void addEntity(Entity* entity);
	void removeEntity(Entity* entity);

	void process();

protected:
	Game* game;
private:
	 std::vector<Entity*> entities;
	 std::bitset<BITSIZE> systemBit;
	 std::bitset<BITSIZE> typeFlags;
};

#endif /* SYSTEM_H_ */
