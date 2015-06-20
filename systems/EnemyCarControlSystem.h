/*
 * EnemyCarControlSystem.h
 *
 *  Created on: 15-apr.-2015
 *      Author: Matthias
 */

#ifndef ENEMYCARCONTROLSYSTEM_H_
#define ENEMYCARCONTROLSYSTEM_H_

#include <bitset>
#include "BitSize.h"
#include "System.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "../components/VelocityComponent.h"
#include "../components/AIComponent.h"
#include "../components/LaneComponent.h"
#include "../components/EnemyCarComponent.h"
#include <SDL2/SDL.h>
#include <chrono>

enum NeighbourBits { NEIGHBORS, FRONT_NEIGHBORS, LEFT_NEIGHBORS, SIDE_NEIGHBORS, RIGHT_NEIGHBORS, BACK_NEIGHBORS, NEIGHBOR_STATES};
enum NeighbourCarPositions { NEIGHBOR_CAR_FRONT, NEIGHBOR_CAR_FRONT_LEFT, NEIGHBOR_CAR_FRONT_RIGHT, NEIGHBOR_CAR_LEFT, NEIGHBOR_CAR_RIGHT, NEIGHBOR_CAR_BACK, NEIGHBOR_CAR_BACK_LEFT, NEIGHBOR_CAR_BACK_RIGHT, NR_OF_NEIGHBORCARPOSITIONS};

class EnemyCarControlSystem : public System {
public:
	EnemyCarControlSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~EnemyCarControlSystem();
private:
	ComponentMapper<AIComponent> aiMapper;
	ComponentMapper<VelocityComponent> velocityMapper;
	ComponentMapper<LaneComponent> laneMapper;
	ComponentMapper<EnemyCarComponent> enemyCarMapper;
	ComponentMapper<PositionComponent> positionMapper;
	std::chrono::time_point<std::chrono::system_clock> lastLaneSwitch;
	std::chrono::time_point<std::chrono::system_clock> lastSpeedChange;

	void sortEnemyCarsInRegionOfInterest(Entity* entity, Entity ** neighboringEnemyCars, std::bitset<NEIGHBOR_STATES> & neigbourStates);
};

#endif /* ENEMYCARCONTROLSYSTEM_H_ */
