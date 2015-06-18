/*
 * SharedCollisionComponent.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Matthias
 */

#ifndef SHAREDCOLLISIONCOMPONENT_H_
#define SHAREDCOLLISIONCOMPONENT_H_

/*This is a component with 1 instance shared by all entities using this component */
#include "IComponent.h"

#include "Utils.h"

class SharedCollisionComponent : public IComponent {
public:
	static SharedCollisionComponent& getInstance() {
		static SharedCollisionComponent  instance;
		return instance;
	}
	void addEntityToCollisionList(Entity * entity) {
		collisionList.push_back(entity);
	}
	void removeEntityFromCollisionList(Entity * entity) {
		for (std::vector<Entity*>::iterator it = collisionList.begin(); it != collisionList.end();) {
			if (entity == (*it))
			{
				//delete *it //test this does not delete entity
				it = collisionList.erase(it);
			}
			else {
				++it;
			}
		}
	}

	std::vector<Entity*>& getCollisionList() {
		return collisionList;
	}

private:
	SharedCollisionComponent() {
	}
	SharedCollisionComponent(SharedCollisionComponent const&) = delete;
	void operator=(SharedCollisionComponent const&) = delete;

	/*due to singleton collisionList can never be destoyed by a destructor so make it static to indicate this */
	std::vector<Entity *> collisionList;

};

#endif /* SHAREDCOLLISIONCOMPONENT_H_ */

