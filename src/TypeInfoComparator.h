/*
 * TypeInfoComparator.h
 *
 *  Created on: 21-jun.-2015
 *      Author: Matthias
 */

#ifndef TYPEINFOCOMPARATOR_H
#define TYPEINFOCOMPARATOR_H

#include <typeinfo>

struct type_info_comparator {
  bool operator ()(const std::type_info* a, const std::type_info* b) const {
    return a->before(*b);
  }
};


#endif /* TYPEINFOCOMPARATOR_H */
