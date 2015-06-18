#ifndef COMPTYPE_H
#define COMPTYPE_H

#include <bitset>
#include "BitSize.h"


class ComponentType {

public:
	ComponentType();
	//==================================
	std::bitset<BITSIZE> getBit() const;
	int getId() const;
	static void reset();
	private:
	//
	static std::bitset<BITSIZE> nextBit;
	static int nextId;
	//==================================
	std::bitset<BITSIZE> bit;
	int id;
	void init();
};

#endif //COMPTYPE_H
