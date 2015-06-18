/*
 * RenderReferenceComponent.h
 *
 *  Created on: 6-apr.-2015
 *      Author: Matthias
 */

#ifndef RENDERREFERENCECOMPONENT_H_
#define RENDERREFERENCECOMPONENT_H_

class RenderReferenceComponent : public IComponent {
public:


	RenderReferenceComponent(Entity * reference) {
		this->reference = reference;
	}

	virtual ~RenderReferenceComponent() {}

	Entity* getReference() {
		return reference;
	}

	void setReference(Entity* reference , Point referenceScreenLocation) {
		this->reference = reference;
		this->referenceScreenLocation = referenceScreenLocation;
	}

	const Point& getReferenceScreenLocation() const {
		return referenceScreenLocation;
	}

	void setReferenceScreenLocation(const Point& referenceScreenLocation) {
		this->referenceScreenLocation = referenceScreenLocation;
	}

private:
	Entity * reference;
	Point referenceScreenLocation;
};

#endif /* RENDERREFERENCECOMPONENT_H_ */
