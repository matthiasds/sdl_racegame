#include "ComponentTypeManager.h"
#include <string>
#include <iostream>
#include "Utils.h"


ComponentType & ComponentTypeManager::getTypeFor(const std::type_info &t) {
	ComponentType * type = componentTypes[&t]; //gets type at index &t or creates index if not exist



	if(type == NULL) {
		type = new ComponentType();
		componentTypes[&t] = type; //gets type
		std::cout << "add new type " <<  t.name() << " to componentTypes"  << std::endl;
	}

	return *type;
};

void ComponentTypeManager::deleteComponentTypes(){

	std::map<const std::type_info*,ComponentType*,
		  type_info_comparator>::iterator it;
	


	for(it = componentTypes.begin(); it != componentTypes.end(); it++)
	{
		delete it->second;
	}
	componentTypes.clear();
}

void ComponentTypeManager::printComponentTypes(){
	//print types
	int nrOfTypes = componentTypes.size();
	std::map<const std::type_info*,ComponentType*,
			  type_info_comparator>::iterator it;
	std::cout << "List of " << nrOfTypes << " Types: " << std::endl;
	for (it=componentTypes.begin(); it !=componentTypes.end(); ++it) {
		ComponentType *mapValue = it->second;
		int valueId = mapValue->getId();
		const std::type_info *mapKey = it->first;
		std::cout << "Key " << 	mapKey->name() << " and value (type->id) : " << valueId << std::endl;
	}

	std::cout << "--------------------------------------------------- " << std::endl;
}

std::map<const std::type_info*,ComponentType*,
	   type_info_comparator> ComponentTypeManager::componentTypes;

