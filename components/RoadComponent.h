/*
 * RoadComponent.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Matthias
 */

#ifndef ROADCOMPONENT_H_
#define ROADCOMPONENT_H_

#include "IComponent.h"


class RoadComponent : public IComponent {
public:
	RoadComponent(int roadWidth, int roadCenter, int Lanes) {
		//set default values
		commingNumberOfLanes = currentNumberOfLanes = 3;
		commingLaneWidth = currentLaneWidth = roadWidth/currentNumberOfLanes;
		commingRoadCenter = currentRoadCenter = roadCenter;
	}
	virtual ~RoadComponent() {
	}
	int getCommingLaneWidth() const {
		return commingLaneWidth;
	}

	void setCommingLaneWidth(int commingLaneWidth) {
		this->commingLaneWidth = commingLaneWidth;
	}

	int getCommingNumberOfLanes() const {
		return commingNumberOfLanes;
	}

	void setCommingNumberOfLanes(int commingNumberOfLanes) {
		this->commingNumberOfLanes = commingNumberOfLanes;
	}

	int getCommingRoadCenter() const {
		return commingRoadCenter;
	}

	void setCommingRoadCenter(int commingRoadCenter) {
		this->commingRoadCenter = commingRoadCenter;
	}

	int getCurrentLaneWidth() const {
		return currentLaneWidth;
	}

	void setCurrentLaneWidth(int currentLaneWidth) {
		this->currentLaneWidth = currentLaneWidth;
	}

	int getCurrentNumberOfLanes() const {
		return currentNumberOfLanes;
	}

	void setCurrentNumberOfLanes(int currentNumberOfLanes) {
		this->currentNumberOfLanes = currentNumberOfLanes;
	}

	int getCurrentRoadCenter() const {
		return currentRoadCenter;
	}

	void setCurrentRoadCenter(int currentRoadCenter) {
		this->currentRoadCenter = currentRoadCenter;
	}

	;
private:
	int commingNumberOfLanes;
	int commingLaneWidth;
	int commingRoadCenter;
	int currentNumberOfLanes;
	int currentLaneWidth;
	int currentRoadCenter;
};

#endif /* ROADCOMPONENT_H_ */
