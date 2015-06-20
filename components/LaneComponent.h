/*
 * RenderReferenceComponent.h
 *
 *  Created on: 6-apr.-2015
 *      Author: Matthias
 */

#ifndef LANECOMPONENT_H_
#define LANECOMPONENT_H_

class LaneComponent : public IComponent {
public:
	LaneComponent(Entity * roadEntityReference) {
		this->roadEntityReference = roadEntityReference;
		currentLane=3;
		wantedLane=3;
		commingNumberOfLanes=3;
	}


	virtual ~LaneComponent() {}

	Entity* getRoadEntityReference() {
		return roadEntityReference;
	}

	void setRoadEntityReference(Entity* roadEntityReference) {
		this->roadEntityReference = roadEntityReference;
	}

	int getCurrentLane() const {
		return currentLane;
	}

	void setCurrentLane(int currentLane) {
		this->currentLane = currentLane;
	}

	int getClosestLane() const {
		return closestLane;
	}

	void setClosestLane(int closestLane) {
		this->closestLane = closestLane;
	}

	int getClosestLaneOffset() const {
		return closestLaneOffset;
	}

	void setClosestLaneOffset(int closestLaneOffset) {
		this->closestLaneOffset = closestLaneOffset;
	}

	int getWantedLane() const {
		return wantedLane;
	}

	void setWantedLane(int wantedLane) {
		this->wantedLane = wantedLane;
	}

	int getCurrentLaneWidth() const {
		return currentLaneWidth;
	}

	void setCurrentLaneWidth(int currentLaneWidth) {
		this->currentLaneWidth = currentLaneWidth;
	}

	int getCommingNumberOfLanes() const {
		return commingNumberOfLanes;
	}

	void setCommingNumberOfLanes(int commingNumberOfLanes) {
		this->commingNumberOfLanes = commingNumberOfLanes;
	}

private:
	Entity * roadEntityReference;
	int currentLane;
	int closestLane;
	int closestLaneOffset;
	int wantedLane;
	int currentLaneWidth;
	int commingNumberOfLanes;

};

#endif /* LANECOMPONENT_H_ */
