//
// Created by ashish on 4/12/16.
//

#include "PhysicsActor.hpp"
#include "b2PhysicsSystem.hpp"
#include "../ModuleSystem/PlayModule.hpp"
#include "../../../rubeStuff/b2dJson.h"
USING_NS_CC;
bool PhysicsActor::init(B2PhysicsSystem *system,const ActorType & type,const b2Vec2 & initPosition) {
    if(!Node::init()) {
        return false;
    }
    this->system = system;
    this->type = type;
    this->initPosition = initPosition;


    return true;
}

void PhysicsActor::onEnter() {
    Node::onEnter();
}






cocos2d::Vec2 PhysicsActor::box2DToActorSpace(const b2Vec2 &bPos) const {
    return convertToNodeSpace(system->box2DToScreen(bPos));

}

cocos2d::Vec2 PhysicsActor::box2DToActorParentSpace(const b2Vec2 bPos) const {
    return _parent->convertToNodeSpace(system->box2DToScreen(bPos));
}


TestActor *TestActor::create(B2PhysicsSystem *system,const b2Vec2 & initPosition) {
    auto tActor = new(std::nothrow)TestActor();
    if(tActor && tActor->init(system,initPosition)){
        tActor->autorelease();
        return tActor;
    }
    CC_SAFE_DELETE(tActor);
    return nullptr;
}

bool TestActor::init(B2PhysicsSystem *system,const b2Vec2 & initPosition) {
    if(!PhysicsActor::init(system,ActorType::Interactive,initPosition)){
        return false;
    }

    {
        b2BodyDef testBodyDef;
        testBodyDef.type = b2_dynamicBody;

        b2PolygonShape p;
        p.SetAsBox(0.5, 0.5);
        bod = system->getWorld()->CreateBody(&testBodyDef);

        bod->SetTransform(b2Vec2(8.5,0.5), 0);
        bod->CreateFixture(&p, 1.0f);
        system->addOffset(bod, initPosition);

        bod->SetActive(false);

        bod->SetUserData(this);

    }

    sprite = Sprite::create("HelloWorld.png");
    sprite->setAnchorPoint(Vec2::ZERO);
    addChild(sprite);










    return true;



}

void TestActor::onEnter() {
    PhysicsActor::onEnter();
    bod->SetActive(true);
    setPosition(box2DToActorParentSpace(bod->GetPosition()));
    setRotation(AngleBToC(bod->GetAngle()));


}


cocos2d::Vec2 TestActor::getDeltaMovement() {
    return cocos2d::Vec2();
}


void TestActor::postPhysicsUpdate(float delta) {


    this->setPosition(box2DToActorParentSpace(bod->GetPosition()));
    this->setRotation(AngleBToC(bod->GetAngle()));




}


TestActor::~TestActor() {

    if(system->isSystemActive()){

        system->getWorld()->DestroyBody(bod);


    }
}



//Test Actor 2


TestActor2 *TestActor2::create(B2PhysicsSystem *system, const b2Vec2 &initPosition) {
    auto tActor2 = new(std::nothrow)TestActor2();
    if(tActor2 && tActor2->init(system,initPosition)){
        tActor2->autorelease();
        return tActor2;
    }
    CC_SAFE_DELETE(tActor2);
    return nullptr;
}

bool TestActor2::init(B2PhysicsSystem *system, const b2Vec2 &initPosition) {
    if(!PhysicsActor::init(system,ActorType ::Interactive,initPosition)){
        return false;
    }
    auto json = system->addJsonObject("Platformer/RubeScenes/test.json");

    {
        mainBody = json.getBodyByName("MainBody");
        circleBody = json.getBodyByName("CircleBody");
        joint = json.getJointByName("joint0");


        mainBody->SetUserData(this);
        circleBody->SetUserData(this);
        joint->SetUserData(this);

        system->addOffset(mainBody, initPosition);
        system->addOffset(circleBody, initPosition);
        system->addOffset(joint, initPosition);

    }

    {
        circleSprite = Sprite::create("HelloWorld.png");
        boxSprite = Sprite::create("HelloWorld.png");
        boxSprite->setAnchorPoint(Vec2::ZERO);
        addChild(boxSprite);
        addChild(circleSprite);
    }




    return true;
}

void TestActor2::onEnter() {
    PhysicsActor::onEnter();

    setPosition(box2DToActorParentSpace(mainBody->GetPosition()));
    setRotation(AngleBToC(mainBody->GetAngle()));

    circleSprite->setPosition(box2DToActorSpace(circleBody->GetPosition()));
    circleSprite->setRotation(AngleBToC(circleBody->GetAngle()));



}

TestActor2::~TestActor2() {


    if(system->isSystemActive()){
        system->getWorld()->DestroyJoint(joint);
        system->getWorld()->DestroyBody(mainBody);
        system->getWorld()->DestroyBody(circleBody);
    }
}

cocos2d::Vec2 TestActor2::getDeltaMovement() {
    return cocos2d::Vec2();
}


void TestActor2::postPhysicsUpdate(float delta) {



    setPosition(box2DToActorParentSpace(mainBody->GetPosition()));
    setRotation(AngleBToC(mainBody->GetAngle()));

    circleSprite->setRotation(AngleBToC(circleBody->GetAngle()));


}

