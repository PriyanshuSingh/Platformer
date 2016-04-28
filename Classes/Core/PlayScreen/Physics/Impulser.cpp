//
// Created by ashish on 4/28/16.
//

#include "Impulser.hpp"
#include "b2PhysicsSystem.hpp"

Impulser::Impulser() {


    system = B2PhysicsSystem::currentInstance;
    system->addImpulser(this);
}

Impulser::~Impulser() {

    system->removeImpulser(this);
}

