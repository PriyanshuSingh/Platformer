//
// Created by ashish on 4/12/16.
//

#include "PhysicsActor.hpp"
#include "b2PhysicsSystem.hpp"
#include "../ModuleSystem/PlayModule.hpp"
#include "../../../rubeStuff/b2dJson.h"
USING_NS_CC;
bool PhysicsActor::init(B2PhysicsSystem *system,PlayModule * parent,const b2Vec2 & initPosition) {
    if(!Node::init()) {
        return false;
    }
    this->system = system;
    this->parentModule = parent;
    this->initPosition = initPosition;
    CCASSERT(parentModule!= nullptr,"parent module null!!");


    return true;
}

void PhysicsActor::onEnter() {
    Node::onEnter();
}


void PhysicsActor::update(float delta) {
    Node::update(delta);
    if(!system->hasUpdated()){
        prePhysicsUpdate(delta);
    }
    else{
        postPhysicsUpdate(delta);
    }


}




cocos2d::Vec2 PhysicsActor::box2DToActorSpace(const b2Vec2 &bPos) const {
    return convertToNodeSpace(system->box2DToScreen(bPos));

}

cocos2d::Vec2 PhysicsActor::box2DToActorParentSpace(const b2Vec2 bPos) const {
    return _parent->convertToNodeSpace(system->box2DToScreen(bPos));
}


TestActor *TestActor::create(B2PhysicsSystem *system,PlayModule * parent,const b2Vec2 & initPosition) {
    auto tActor = new(std::nothrow)TestActor();
    if(tActor && tActor->init(system,parent,initPosition)){
        tActor->autorelease();
        return tActor;
    }
    CC_SAFE_DELETE(tActor);
    return nullptr;
}

bool TestActor::init(B2PhysicsSystem *system,PlayModule * parent,const b2Vec2 & initPosition) {
    if(!PhysicsActor::init(system,parent,initPosition)){
        return false;
    }


    type = ActorType::Void;
    {
        b2BodyDef testBodyDef;
        testBodyDef.type = b2_dynamicBody;

        b2PolygonShape p;
        p.SetAsBox(0.5, 0.5);
        bod = system->getWorld()->CreateBody(&testBodyDef);


        bod->SetTransform(b2Vec2(8,0.5), 0);
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


TestActor2 *TestActor2::create(B2PhysicsSystem *system, PlayModule *parent, const b2Vec2 &initPosition) {
    auto tActor2 = new(std::nothrow)TestActor2();
    if(tActor2 && tActor2->init(system,parent,initPosition)){
        tActor2->autorelease();
        return tActor2;
    }
    CC_SAFE_DELETE(tActor2);
    return nullptr;
}

bool TestActor2::init(B2PhysicsSystem *system, PlayModule *parent, const b2Vec2 &initPosition) {
    if(!PhysicsActor::init(system, parent, initPosition)){
        return false;
    }
    b2dJson json;
    std::string errMsg;
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile("platformer/rubeScenes/test.json");
    json.readFromString(jsonContent, errMsg,system->getWorld());

#ifdef DEBUGGING_APP

    if(errMsg.empty()){
        cocos2d::log("no error occured json loaded ok");
    }
    else{
        cocos2d::log("Error occured : %s",errMsg.c_str());
    }
#endif

    mainBody = json.getBodyByName("MainBody");
    circleBody = json.getBodyByName("CircleBody");




    system->addOffset(mainBody,initPosition);
    system->addOffset(circleBody,initPosition);



    circleSprite = Sprite::create("CircleBtn.png");
    boxSprite = Sprite::create("HelloWorld.png");
    boxSprite->setAnchorPoint(Vec2::ZERO);
    addChild(boxSprite);
    addChild(circleSprite);









    return true;
}

void TestActor2::onEnter() {
    PhysicsActor::onEnter();

    box2DToActorParentSpace(mainBody->GetPosition());



}

TestActor2::~TestActor2() {

}

cocos2d::Vec2 TestActor2::getDeltaMovement() {
    return cocos2d::Vec2();
}


void TestActor2::postPhysicsUpdate(float delta) {



    this->setPosition(box2DToActorParentSpace(mainBody->GetPosition()));
    this->setRotation(AngleBToC(mainBody->GetAngle()));

    circleSprite->setPosition(box2DToActorSpace(circleBody->GetPosition()));
    circleSprite->setRotation(AngleBToC(circleBody->GetAngle()));

}

