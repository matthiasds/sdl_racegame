/*
 * CarLaneMovingSystem.h
 *
 *  Created on: 8-apr.-2015
 *      Author: Matthias
 */


#ifndef CARLANEMOVINGSYSTEM_H_
#define CARLANEMOVINGSYSTEM_H_

#include <bitset>
#include "BitSize.h"
#include "System.h"
#include "Entity.h"
#include "ComponentMapper.h"
#include "../components/InputComponent.h"
#include "../components/LaneComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RoadComponent.h"

enum LaneMovingStates { LANE_FOLLOWING, OUT_OF_LANE_FOLLOWING };

class CarLaneMovingSystem : public System {
public:
	CarLaneMovingSystem();
	void init() override;
	void processEntity(Entity* entity) override;
	virtual ~CarLaneMovingSystem();


private:
	ComponentMapper<LaneComponent> LaneMapper;
	ComponentMapper<PositionComponent> positionMapper;
	ComponentMapper<RoadComponent> RoadMapper;
	ComponentMapper<VelocityComponent> velocityMapper;

	/* this data block, even the states is not in here to be saved, only to share between the class methods */
	LaneMovingStates laneMovingState;
	int currentRoadCenter;
	int currentNumberOfLanes;
	int currentLaneWidth;
	int commingRoadCenter;
	int commingNumberOfLanes;
	int commingLaneWidth;
	std::vector<int> currentCenterLanePositions;
	std::vector<int> commingCenterLanePositions;

	void updateLaneInfo(Entity* entity);
	void getClosestCurrentLaneAndOffset(int &entityPosition, int &offset, int &closestLaneNumber);
	void getClosestCommingLaneAndOffset(int &entityPosition, int &offset, int &closestLaneNumber);

};



#endif /* CARLANEMOVINGSYSTEM_H_ */
