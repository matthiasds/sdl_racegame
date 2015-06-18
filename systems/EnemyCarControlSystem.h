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
#include <SDL2/SDL.h>
#include <chrono>


class EnemyCarControlSystem : public System {
public:
	EnemyCarControlSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~EnemyCarControlSystem();
private:
	ComponentMapper<AIComponent> aiMapper;
	ComponentMapper<VelocityComponent> velocityMapper;
	ComponentMapper<LaneComponent> LaneMapper;
	int wantedLane;
	std::chrono::time_point<std::chrono::system_clock> lastLaneSwitch;
	std::chrono::time_point<std::chrono::system_clock> lastSpeedChange;
};

#endif /* ENEMYCARCONTROLSYSTEM_H_ */
