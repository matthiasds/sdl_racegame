/*
 * PositionComponent.h
 *
 *  Created on: 6-apr.-2015
 *      Author: Matthias
 */

#ifndef POSITIONCOMPONENT_H_
#define POSITIONCOMPONENT_H_


class PositionComponent : public IComponent {
public:


	PositionComponent(FPoint position) {
		this->position.push_back(position);
		rotation = 0;
	}

	std::vector<FPoint>& getPositionVector(){
			return position;
	}

	FPoint getPosition(){
		return position.at(0);
	}

	void setPosition(const FPoint position) {
		this->position.at(0) = position;
	}

	int getRotation() const {
		return rotation;
	}

	void setRotation(int rotation) {
		this->rotation = rotation;
	}

	virtual ~PositionComponent() {};
private:
	std::vector<FPoint> position;
	int rotation;
};

#endif /* POSITIONCOMPONENT_H_ */
