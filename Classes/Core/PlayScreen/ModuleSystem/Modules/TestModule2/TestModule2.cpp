//
// Created by Priyanshu Singh on 03/05/16.
//

#include "TestModule2.hpp"
#include "LightHandler.hpp"
#include "b2PhysicsSystem.hpp"

using namespace box2dLight;
void TestModule2::BeginContact(b2Contact *contact) {
    PlayModule::BeginContact(contact);
}

void TestModule2::EndContact(b2Contact *contact) {
    PlayModule::EndContact(contact);
}

bool TestModule2::init(const PlayModule::staticInfo &info, B2PhysicsSystem *system, MainCamera *cam, const b2Vec2 &offset) {
    if(!PlayModule::init(info, system, cam, offset))return false;


    return true;
}

void TestModule2::prePhysicsUpdate(float delta) {
    PlayModule::prePhysicsUpdate(delta);
    lightHandler->update(delta);
}

void TestModule2::postPhysicsUpdate(float delta) {
    PlayModule::postPhysicsUpdate(delta);
}

void TestModule2::onCoordsStable() {
    PlayModule::onCoordsStable();
}

void TestModule2::onEnter() {
    Node::onEnter();
    lightHandler = LightHandler::create(system->getWorld());
    auto size = Director::getInstance()->getWinSize();
    pointLight = box2dLight::PointLight::create(lightHandler, 1000, Color4F(1,1,1,1), 500.0f/64.0f, 0.0f);
    pointLight->setStartPosition(size.width/128, size.height/128);
    addChild(lightHandler, 200);
    pointLight->setCameraMask((unsigned short) CameraFlag::USER1);
    lightHandler->setCameraMask((unsigned short) CameraFlag::USER1);

}
