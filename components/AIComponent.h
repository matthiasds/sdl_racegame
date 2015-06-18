/*
 * AIComponent.h
 *
 *  Created on: 6-apr.-2015
 *      Author: Matthias
 */

#ifndef AICOMPONENT_H_
#define AICOMPONENT_H_

#include "IComponent.h"

class AIComponent : public IComponent {
public:

	AIComponent() {
		active=true;
	}

	virtual ~AIComponent() {
	}

private:
	bool active;
};



#endif /* AICOMPONENT_H_ */
