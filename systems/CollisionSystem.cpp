/*
 * CollisionSystem.cpp
 *
 *  Created on: 7-apr.-2015
 *      Author: Matthias
 */

#include "CollisionSystem.h"
#include "Game.h"

/**
 * CollisionSystem:
 * This Systems task is to detect collisions between all entities with the components renderable, position, collision and velocity.
 * The reference component is used to only do this detection in a certain window (ROI) around the reference. This reduces CPU load as the
 * CollisionSystem does the most calculations of all systems (for loops with checks). Add to this collisions normally only happen by the
 * chaotic impact of the player car.
 * Ahead of the player car, enemy cars should behave well enough with their EnemyCarControlSystem which implements collision prevention.
 *
 * When an collision is detected, the parameters like speedOfImpact in each direction are saved in the CollisionComponent.
 * These saved parameters are used to calculate the "bumping" of cars and the damage applied to cars by the DamageSystem.
 * The faster the impact the more damage and harder the bumping.
 *
 * As the processEntity function can only execute at 1 entity at a time there was a problem to analyze the interactions with the other entities
 * In most Component Entity Systems this is solved by other Systems and group managers that can behave on multiple entities. Here I have created
 * an own way of dealing with this deviating from the standard CES pattern. It works by creating an extra SharedCollisionComponent which is created as
 * singleton. Each entity creating the CollisionComponent is added to an entityList in the SharedCollisionComponent. Each CollisionComponent contains
 * a pointer to the sharedCollisionComponent so the entityList can be accessed from each component. In this way they entities subscribing to the same
 * component are aware of each other. In this way the component function is recycled in a creative way to prevent adding another complicated CES part
 * like the group manager.
 * Only future can tell if this deviation can work in all situations (new design patterns can only be proven to work by extensive use in different
 * applications). But in this rather limited game it works out quite well.
 */


/**
 * Constructor: The Components below are the components to which this system is subscribed.
 * This System is applied to ALL entities having (minimum) ALL of the Components below.
 */
CollisionSystem::CollisionSystem() {
	addComponentType<SdlRenderComponent>();
	addComponentType<PositionComponent>();
	addComponentType<RenderReferenceComponent>();
	addComponentType<CollisionComponent>();
	addComponentType<VelocityComponent>();
}

/**
 * processEntity: This function is executed for all entities (individual = one by one) that contain the Components
 * the System is subscribed to in the constructor (above).
 * If you want to make use of this System on a certain entity. The components described in the constructor should be added
 * to the Entity in the (Sdl)EntityFactory.
 * @param entity: the pointer to the current entity (an entity is used as container of all components and has a unique id)
 */
void CollisionSystem::processEntity(Entity* entity) {
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

						int speedOfImpactX = velocityMapper.get(currentEntity)->getVelocityX() - velocityMapper.get(entity)->getVelocityX(); //positive = impact to me, negative, impact from me
						int speedOfImpactY = velocityMapper.get(currentEntity)->getVelocityY() - velocityMapper.get(entity)->getVelocityY();
						collisionMapper.get(entity)->setCollisionStatus(NEW_COLLISION, currentEntity, collisionType, pointOfImpact, speedOfImpactX, speedOfImpactY);
					}
					collisionMapper.get(entity)->updatePointOfImpact(pointOfImpact);
				}
				//if handling is done by MovementSystem only CollisionSystem can decide to release collision
				else if (collisionMapper.get(entity)->getCollisionStatus().collisionHandlingState == COLLISION_HANDLING_DONE) {

						collisionMapper.get(entity)->setCollisionStatus(NO_COLLISION, NULL, 0, pointOfImpact, 0 , 0);
				}
			}
		}
	}

}


/**
 * This function checks if an entity is in the Region Of Interest (ROI) of the reference entity (usually player Car)
 * If so a boolean true is returned to indicated further collision calculations make sense. This is done to reduce the collision
 * work only to a window area around the player car. As the player car (reference entity) is the main source of chaos and collisions.
 *
 * @param entity: entity is always needed as argument as it is the container for all components (in this case entity position)
 * @param referencePosition: Y position of the reference
 * @return boolean value indicating an entity is in ROI of reference and further collision calculations should be done
 */
bool CollisionSystem::isEntityInRegionOfInterest(Entity* entity , int referencePosition) {
	static int WindowsizeH = 0;
	if (WindowsizeH == 0) {
		Rect screenSize=game->getScreen();
		WindowsizeH = screenSize.h;
	}
	FPoint enityPosition = positionMapper.get(entity)->getPosition();
	if (std::abs(enityPosition.y - referencePosition) < WindowsizeH*2 ) {
		return true;
	}
	return false;
}

/**
 * This function uses a pre-caluclated Point Of Impact and the collisionBox representing the shape and position of the car
 * to determine the type of collision (front, back, right, left) by using a bitset combinations are possible (ex. FRONT+LEFT)
 * @param collisionBox: This argument is the collision box representing the shape of the car.
 * @param pointOfImpact: This argument is the precaculated Point Of Impact.
 * @return bitset collisionType: The type of collision (back, front, left, right) is returned in a bitset
 */
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

/**
 * This function calculates the Point Of Impact of 2 rectangles if they collide. False is returned
 * when they do not collide at all
 * @param a: collision rectangle a representing the shape and position of the 1st entity
 * @param b: collision rectangle b representing the shape and position of the 1st entity
 * @param pointOfImpact: on return this is filled with the center coordinates of the collision -> return
 * @return boolean: returns false if there is no collision at  all between the collision rectangles
 */
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


/**
 * Destructor: As Systems generally do not contain data (only Components do) the Systems destructor doesn't have a function
 */
CollisionSystem::~CollisionSystem() {
}


/**
* init: not used in this System
*/
void CollisionSystem::init() {
}
