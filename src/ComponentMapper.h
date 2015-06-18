#ifndef COMPONENTMAPPER_H
#define COMPONENTMAPPER_H

#include "Entity.h"

  template<typename component>
  /**
   * High performance component retrieval from entities. Use this wherever you
   * need to retrieve components from entities often and fast.
   * This is a solution to prevent problems with component templates and order of compilation...
   */
  class ComponentMapper {
  public:
     ~ComponentMapper() {}
    
    /**
     *Returns the component mapped to the Entity.
     *If there is no such component associated with the entity
     *NULL is returned.
     */
    component * get(Entity * entity) {
      //ATTN perhaps pointing to the component bag is faster.
      return (component*)entity->getComponent<component>();
    }
    
  };

#endif // COMPONENTMAPPER_H
