#include "RoadInputComponent.h"


#include "Entity.h"
#include "Utils.h"

RoadInputComponent::RoadInputComponent() {
	// TODO Auto-generated constructor stub

}

void RoadInputComponent::update(Entity& entity) {
	Point position = entity.getPos();
	position.y+=1;
	entity.setPos(position);


}

RoadInputComponent::~RoadInputComponent() {
	// TODO Auto-generated destructor stub
}

