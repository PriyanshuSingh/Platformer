//
// Created by ashish on 5/21/16.
//

#include "PhysicsUpdatable.hpp"
#include "b2PhysicsSystem.hpp"

PhysicsUpdatable::PhysicsUpdatable():system(B2PhysicsSystem::currentInstance) {
    CCASSERT(system!= nullptr,"Making actor when no Physics System!");

    system->addPhysicsUpdatable(this);

}


PhysicsUpdatable::~PhysicsUpdatable() {
    system->removePhysicsUpdatable(this);
}

