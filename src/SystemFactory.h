/*
 * SystemFactory.h
 *
 *  Created on: 8-apr.-2015
 *      Author: Matthias
 */

#ifndef SYSTEMFACTORY_H_
#define SYSTEMFACTORY_H_

class ISystem;

class SystemFactory {
public:
	SystemFactory();
	void setSystem(ISystem* system);
	virtual ~SystemFactory();
};

#endif /* SYSTEMFACTORY_H_ */
