//
// Created by ashish on 4/28/16.
//

#include "Impulser.hpp"
#include "b2PhysicsSystem.hpp"


bool Impulser::init() {

    system = B2PhysicsSystem::currentInstance;
    system->addImpulser(this);
    return true;
}


Impulser::~Impulser() {

    system->removeImpulser(this);
}

