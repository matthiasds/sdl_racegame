/*
 * Entity.h
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#ifndef ENTITY_H_
#define ENTITY_H_


#include "Utils.h"
#include "IComponent.h"
#include <bitset>
#include <vector>
#include <map>
#include "ComponentType.h"
#include "BitSize.h"
#include "ComponentTypeManager.h"

class Game;
class Renderer;

class Entity {
public:
	Entity(Game* game, int id);
	void reset();
	Point getPos() const;
	uint32_t getPrio() const;
	void addComponent(IComponent * c);

	template<typename component>
	IComponent* getComponent() {
		ComponentType type = ComponentTypeManager::getTypeFor<component>();
		 return getComponent(&type);
	}

	std::bitset<BITSIZE> getTypeBits();
	std::bitset<BITSIZE> getSystemBits();
	void addSystemBit(std::bitset<BITSIZE> bit);

	void refresh();
	void removeComponent(ComponentType & type) ;
	void update();
	virtual ~Entity();
	void setPos(const Point& pos);
	void setPrio(uint32_t prio);
	void Reset();
	long int getUniqueId() const;
	void setUniqueId(long int uniqueId);
	Game* getGame() const;
	int getId() const;
	void setId(int id);

private:
	IComponent * getComponent(ComponentType * type); //forward function for template getComponent
	Point pos;
	uint32_t prio;

	Game *game;
	int id;
	long int uniqueId;
	std::bitset<BITSIZE> typeBits;
	std::bitset<BITSIZE> systemBits;
};

#endif /* ENTITY_H_ */

