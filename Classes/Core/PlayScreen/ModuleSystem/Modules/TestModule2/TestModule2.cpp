//
// Created by Priyanshu Singh on 03/05/16.
//

#include "TestModule2.hpp"

void TestModule2::BeginContact(b2Contact *contact) {
    PlayModule::BeginContact(contact);
}

void TestModule2::EndContact(b2Contact *contact) {
    PlayModule::EndContact(contact);
}

bool TestModule2::init(const PlayModule::staticInfo &info, B2PhysicsSystem *system, MainCamera *cam, const b2Vec2 &offset) {
    return PlayModule::init(info, system, cam, offset);
}

void TestModule2::prePhysicsUpdate(float delta) {
    PlayModule::prePhysicsUpdate(delta);
}

void TestModule2::postPhysicsUpdate(float delta) {
    PlayModule::postPhysicsUpdate(delta);
}

void TestModule2::onCoordsStable() {
    PlayModule::onCoordsStable();
}
