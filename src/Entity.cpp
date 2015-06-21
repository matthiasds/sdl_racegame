/*
 * Entity.cpp
 *
 *  Created on: 23-mrt.-2015
 *      Author: Matthias
 */

#include "Entity.h"
#include "ComponentType.h"
#include "ComponentTypeManager.h"
#include "Game.h"
#include "../components/VelocityComponent.h"
#include "BitSize.h"
#include "Utils.h"




Entity::Entity(Game* game, int id)
{
	this->game = game;
	this->id = id;
}

Entity::~Entity() {
	// TODO Auto-generated destructor stub
}



Point Entity::getPos() const {
	return pos;
}


uint32_t Entity::getPrio() const {
	return prio;
}


void Entity::update() {
	game->updateEntity(*this);
}

void Entity::setPos(const Point& pos) {
	this->pos = pos;
}

void Entity::addComponent(IComponent* component) {
	 ComponentType type = ComponentTypeManager::getTypeFor(typeid(*component));
	 std::map<int, std::vector<IComponent*> *>& componentsByType = game->getComponentsByType();


	 if (Utils::getInstance().isDebuggingEnabled()) {
		 ComponentTypeManager::printComponentTypes();
		 std::cout << "AddComponent of type " << type.getId() << " AKA " << typeid(*component).name() << " to Entity with id " << id << std::endl;
		 std::cout << "---------------------------------------------------" << std::endl;
	 }
	 std::vector<IComponent*> * currentComponents;

	 if (componentsByType.count(type.getId()) == 0) // check if map is not empty and type is in map
	 { //key does not exist in map
		 //create new vector with size of possible components
		 currentComponents = new std::vector<IComponent*>();
		//currentComponents = & currentComponentVector;
		componentsByType.insert(std::make_pair(type.getId(), currentComponents));
	 } //key exists in map
	 else
	 {
		 currentComponents = componentsByType[type.getId()];
	   if (this->id < (int) currentComponents->size())
	   {
		 // Entity already had this component, need to perform component removal first
		 removeComponent(type);
		 //refresh(e);
	   }
	 }
	 //if id exists
	 if (id >= (int) currentComponents->size())
	 {
		 currentComponents->resize(id);
	 	 currentComponents->insert(currentComponents->begin()+id,component);
	 }
	 else
	 {
		 currentComponents->at(id) = component;
	 }
	     typeBits |= type.getBit();

	if (Utils::getInstance().isDebuggingEnabled()) {
	 game->printComponentsByType();
	}

}



void Entity::removeComponent(ComponentType & type) {
  std::vector<IComponent* > * componentsOfType = game->getComponentsByType()[type.getId()];

  delete &componentsOfType[this->id];
  //componentsOfType[this->id]= NULL; //TODO debug to check this
  typeBits &= ~(type.getBit());
  componentsOfType = NULL;
};



void Entity::setPrio(uint32_t prio) {
	this->prio = prio;
}

long int Entity::getUniqueId() const {
	return uniqueId;
}

void Entity::reset() {
	 typeBits = 0;
	 systemBits = 0;
}

std::bitset<BITSIZE> Entity::getSystemBits() {
   return systemBits;
 }

std::bitset<BITSIZE> Entity::getTypeBits() {
   return typeBits;
 }

void Entity::addSystemBit(std::bitset<BITSIZE> bit) {
	systemBits |= bit;
}

IComponent * Entity::getComponent(ComponentType * type) {
	IComponent * component = NULL;
	std::vector<IComponent*> * componentsOfType = game->getComponentsByType()[type->getId()];
	if(componentsOfType != NULL && id < (int) componentsOfType->size()) {
		component = (*componentsOfType)[id];
	}
	return component;
};

void Entity::setUniqueId(long int uniqueId) {
	this->uniqueId = uniqueId;
}

Game* Entity::getGame() const {
	return game;
}

int Entity::getId() const {
	return id;
}

void Entity::setId(int id) {
	this->id = id;
}
