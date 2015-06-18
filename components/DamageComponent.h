/*
 * DamageComponent.h
 *
 *  Created on: 11-may.-2015
 *      Author: Matthias
 */

#ifndef DAMAGECOMPONENT_H_
#define DAMAGECOMPONENT_H_

class DamageComponent : public IComponent {
public:


	DamageComponent() {
		damage = 0;
	}

	virtual ~DamageComponent() {
	}
	int getDamage() const {
		return damage;
	}

	void setDamage(int damage) {
		this->damage = damage;
	}

private:
	int damage;
};

#endif /* DAMAGECOMPONENT_H_ */
