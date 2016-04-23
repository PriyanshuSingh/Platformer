//
// Created by ashish on 4/12/16.
//

#include "PhysicsActor.hpp"
#include "b2PhysicsSystem.hpp"
#include "../ModuleSystem/PlayModule.hpp"
USING_NS_CC;
bool PhysicsActor::init(B2PhysicsSystem *system,PlayModule * parent,const b2Vec2 & initOffset) {
    if(!Node::init()) {
        return false;
    }
    this->system = system;
    this->parentModule = parent;
    CCASSERT(parentModule!= nullptr,"parent module null!!");


    return true;
}


void PhysicsActor::setModuleActive(bool stable) {

    if(stable == this->stable)return;
    this->stable = stable;
    if(stable)onModuleActive();
}


TestActor *TestActor::create(B2PhysicsSystem *system,PlayModule * parent,const b2Vec2 & initOffset) {
    auto tActor = new(std::nothrow)TestActor();
    if(tActor && tActor->init(system,parent,initOffset)){
        tActor->autorelease();
        return tActor;
    }
    CC_SAFE_DELETE(tActor);
    return nullptr;
}

bool TestActor::init(B2PhysicsSystem *system,PlayModule * parent,const b2Vec2 & initOffset) {
    if(!PhysicsActor::init(system,parent,initOffset)){
        return false;
    }


    type = ActorType::Void;
    {
        b2BodyDef testBodyDef;
        testBodyDef.type = b2_dynamicBody;

        b2PolygonShape p;
        p.SetAsBox(0.5, 0.5);
        bod = system->getWorld()->CreateBody(&testBodyDef);


        bod->SetTransform(b2Vec2(8,8), 0);
        bod->CreateFixture(&p, 1.0f);
        system->addOffset(bod, initOffset);

        bod->SetActive(false);

        bod->SetUserData(this);

    }

    sprite = Sprite::create("HelloWorld.png");
    sprite->setAnchorPoint(Vec2::ZERO);
    this->addChild(sprite);




    this->setPosition(parentModule->tranformBox2DToModule(bod->GetPosition()));
    this->setRotation(-1*CC_RADIANS_TO_DEGREES(bod->GetAngle()));





    return true;



}


cocos2d::Vec2 TestActor::getDeltaMovement() {
    return cocos2d::Vec2();
}


void TestActor::postPhysicsUpdate(float delta) {
    this->setPosition(parentModule->tranformBox2DToModule(bod->GetPosition()));
    this->setRotation(-1*CC_RADIANS_TO_DEGREES(bod->GetAngle()));


//    PlatformerGlobals::printVec2("position is ",getParent()->convertToWorldSpace(getPosition()));
//    PlatformerGlobals::printVec2("sprite position is ",sprite->getParent()->convertToWorldSpace(sprite->getPosition()));


}


TestActor::~TestActor() {

    if(system->isSystemActive()){

        system->getWorld()->DestroyBody(bod);


    }
}


void TestActor::onModuleActive() {

    bod->SetActive(true);
}

