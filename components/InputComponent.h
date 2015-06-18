/*
 * InputComponent.h
 *
 *  Created on: 6-apr.-2015
 *      Author: Matthias
 */

#ifndef INPUTCOMPONENT_H_
#define INPUTCOMPONENT_H_
enum Inputs { UP, DOWN , LEFT, RIGHT, IDLE };

class InputComponent : public IComponent {
public:

	InputComponent() {
		inputs = IDLE;
	}

	virtual ~InputComponent() {
	}

	Inputs getInputs() const {
		return inputs;
	}

	void setInputs(Inputs inputs) {
		this->inputs = inputs;
	}

	;
private:
	Inputs inputs;

};



#endif /* INPUTCOMPONENT_H_ */
