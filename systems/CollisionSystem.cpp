/*
 * CollisionSystem.cpp
 *
 *  Created on: 7-apr.-2015
 *      Author: Matthias
 */

#include "CollisionSystem.h"
#include "Game.h"

CollisionSystem::CollisionSystem() {
	addComponentType<RenderComponent>();
	addComponentType<PositionComponent>();
	addComponentType<RenderReferenceComponent>();
	addComponentType<CollisionComponent>();
	addComponentType<VelocityComponent>();
}

void CollisionSystem::processEntity(Entity* entity) {

	//std::sort(entitiesToUpdate.begin() , entitiesToUpdate.end(), priocompare);

	std::vector<Entity*> collisionList = collisionMapper.get(entity)->getCollisionList();


	Entity* reference = renderReferenceMapper.get(entity)->getReference();
	FPoint referencePosition = positionMapper.get(reference)->getPosition();




	if (isEntityInRegionOfInterest(entity, referencePosition.y )) {  //check if entity is on a position close to what is visible
		//get properties of the entity to match against all other entities
		Rect entityCollisionBox = collisionMapper.get(entity)->getCollisionRect();
		FPoint myPosition = positionMapper.get(entity)->getPosition();
		entityCollisionBox.x = myPosition.x;
		entityCollisionBox.y = myPosition.y;

		for (auto const &currentEntity : collisionList) {
			if (isEntityInRegionOfInterest(currentEntity, referencePosition.y ) && currentEntity!=entity) { //check if other entity to check is close to visible and it is not myself

				//get properties of the current entity to match with
				Rect currentEntityCollisionBox = collisionMapper.get(currentEntity)->getCollisionRect();
				FPoint currentEnityPosition = positionMapper.get(currentEntity)->getPosition();
				currentEntityCollisionBox.x = currentEnityPosition.x;
				currentEntityCollisionBox.y = currentEnityPosition.y;
				Point pointOfImpact;

				if (checkCollision(entityCollisionBox, currentEntityCollisionBox, pointOfImpact)) {
					if (collisionMapper.get(entity)->getCollisionStatus().collisionHandlingState == NO_COLLISION) { //only for new collisons

						//analyze collisiontype
						std::bitset<MAX_NR_OF_COLLISIONTYPES> collisionType = analyzeCollisionType(entityCollisionBox, pointOfImpact);

						int speedOfImpactX = velocityMapper.get(currentEntity)->getVelocityX() - velocityMapper.get(entity)->getVelocityX(); //positive = impact to me, negatve, impact from me
						int speedOfImpactY = velocityMapper.get(currentEntity)->getVelocityY() - velocityMapper.get(entity)->getVelocityY();
						collisionMapper.get(entity)->setCollisionStatus(NEW_COLLISION, currentEntity, collisionType, pointOfImpact, speedOfImpactX, speedOfImpactY);

						std::cout << "Collision of entity id " << entity->getId() << " with entity id " << currentEntity->getId() << std::endl;
						std::cout << "SpeedOfImpact X: " << speedOfImpactX << " ,SpeedOfImpact Y: " << speedOfImpactY << std::endl;
					}
					collisionMapper.get(entity)->updatePointOfImpact(pointOfImpact);


				}
				//if handling is done by MovementSystem only CollisionSystem can decide to release collision
				else if (collisionMapper.get(entity)->getCollisionStatus().collisionHandlingState == COLLISION_HANDLING_DONE) {
						std::cout << "end collision " << entity->getId() << " with entity id " << currentEntity->getId() << std::endl;
						collisionMapper.get(entity)->setCollisionStatus(NO_COLLISION, NULL, 0, pointOfImpact, 0 , 0);
				}
			}
		}
	}

}

bool CollisionSystem::isEntityInRegionOfInterest(Entity* entity , int referencePosition) {
	static int WindowsizeH = 0;
	if (WindowsizeH == 0) {
		Rect screenSize=game->getRenderer()->getContextSize();
		WindowsizeH = screenSize.h;
	}
	FPoint enityPosition = positionMapper.get(entity)->getPosition();
	if (std::abs(enityPosition.y - referencePosition) < WindowsizeH*2 ) {
		return true;
	}
	return false;
}


std::bitset<MAX_NR_OF_COLLISIONTYPES> CollisionSystem::analyzeCollisionType(Rect collisionBox, Point& pointOfImpact) {
	std::bitset<MAX_NR_OF_COLLISIONTYPES> collisionType;


	//test for front back collision
	if (pointOfImpact.y <= (collisionBox.y-collisionBox.h*0.4) ) {  //a car collides in my back if hit at lowest 10% zone of collisionBox
		collisionType.set(BACK_COL);
	}
	else if  (pointOfImpact.y >= (collisionBox.y+collisionBox.h*0.4)  ){ //my car collides in another cars back if hit at highest 10% zone of collisionBox
		collisionType.set(FRONT_COL);
	}

	//test for side collision, can  be combined with front back to detect corner hits
	if (pointOfImpact.x <= (collisionBox.x-collisionBox.w*0.4) ) {  //the collision is in my left if hit is at 10% most left zone of collisionBox
			collisionType.set(LEFT_COL);
	}
	else if  (pointOfImpact.x >= (collisionBox.x+collisionBox.w*0.4)  ){ //my car collides in another cars back if hit at highest 10% zone of collisionBox
		collisionType.set(RIGHT_COL);
	}


	return collisionType;
}



/* arguments Rect a and b are  the collision rectangles of the 2 entities, the collisionPoint is filled with the center coordinates of the collision */


bool CollisionSystem::checkCollision( Rect a, Rect b, Point& pointOfImpact ) {
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    /* start with center coordinates, if no collision */
    pointOfImpact.x = a.x;
    pointOfImpact.y = a.y;


    //Calculate the sides of rect A
    leftA = a.x-a.w/2;
    rightA = a.x + a.w/2;
    topA = a.y - a.h/2;
    bottomA = a.y + a.h/2;

    //Calculate the sides of rect B
    leftB = b.x -b.w/2;
    rightB = b.x + b.w/2;
    topB = b.y - b.h/2;
    bottomB = b.y + b.h/2;

    //If any of the sides from A are outside of B
	if( bottomA <= topB )
	{
	  return false;
	}

	if( topA >= bottomB )
	{
	  return false;
	}

	if( rightA <= leftB )
	{
	  return false;
	}

	if( leftA >= rightB )
	{
	  return false;
	}

	pointOfImpact.x += (b.x - a.x )/2;
	pointOfImpact.y += (b.y - a.y )/2;

	//If none of the sides from A are outside B
	return true;
}


CollisionSystem::~CollisionSystem() {
	// TODO Auto-generated destructor stub
}



void CollisionSystem::init() {
}
