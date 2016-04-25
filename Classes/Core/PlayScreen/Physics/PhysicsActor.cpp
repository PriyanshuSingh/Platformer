//
// Created by ashish on 4/12/16.
//

#include "PhysicsActor.hpp"
#include "b2PhysicsSystem.hpp"
#include "../ModuleSystem/PlayModule.hpp"
#include "../../../rubeStuff/b2dJson.h"
USING_NS_CC;


void PhysicsActor::setUserData() {

    for(auto & b:bodies){
        b->SetUserData(this);
    }
    for(auto & j:joints){
        j->SetUserData(this);
    }



}
void PhysicsActor::setAbsPosition(const b2Vec2 & pos) {
    for(auto & b:bodies){
        system->addOffset(b,pos);
    }
    for(auto & j:joints){
        system->addOffset(j,pos);
    }
}
void PhysicsActor::setBodiesActive(bool flag) {

    for(auto & b:bodies){
        b->SetActive(flag);
    }

}


void PhysicsActor::deleteAllBodiesAndJoints() {

    if(system->isSystemActive()){
        //joints before bodies
        for(auto &j :joints){
            system->getWorld()->DestroyJoint(j);
        }
        for(auto &b :bodies){
            system->getWorld()->DestroyBody(b);
        }
        joints.clear();
        bodies.clear();

    }


}


bool PhysicsActor::init(B2PhysicsSystem *system,const ActorType & type,const b2Vec2 & initPosition,const bVec & bodies,const jVec & joints) {

    this->joints = joints;
    this->bodies = bodies;
    return commonInit(system,type,initPosition);
}

bool PhysicsActor::init(B2PhysicsSystem *system, const PhysicsActor::ActorType &type, const b2Vec2 &initPosition,b2dJson &json) {
    json.getAllBodies(bodies);
    json.getAllJoints(joints);
    return commonInit(system,type,initPosition);
}
bool PhysicsActor::commonInit(B2PhysicsSystem *system,const ActorType & type,const b2Vec2 & initPosition){


    if(!Node::init()) {
        return false;

    }


    setTag(DontCareTag);
    this->system = system;
    this->type = type;

    setupPhysicsObjects(initPosition,false);
    return true;


}


void PhysicsActor::setupPhysicsObjects(const b2Vec2 & pos,bool active) {
    setAbsPosition(pos);
    setUserData();
    setBodiesActive(active);

}


void PhysicsActor::onEnter() {
    Node::onEnter();
    setBodiesActive(true);
}


PhysicsActor::~PhysicsActor() {
    deleteAllBodiesAndJoints();
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


        b2BodyDef testBodyDef;
        testBodyDef.type = b2_dynamicBody;

        b2PolygonShape p;
        p.SetAsBox(0.8f/2,0.8f/2);
        bod = system->getWorld()->CreateBody(&testBodyDef);

        b2FixtureDef fixDef;
        fixDef.shape = &p;
        fixDef.density= 5/0.64f;
        fixDef.friction = 0.01;
        bod->CreateFixture(&fixDef);


    bVec bods;
    bods.push_back(bod);
    jVec jos;
    if(!PhysicsActor::init(system,ActorType::Interactive,initPosition,bods,jos)){
        return false;
    }

    sprite = Sprite::create("HelloWorld.png");
    sprite->setAnchorPoint(Vec2::ZERO);
    addChild(sprite);



    return true;



}

void TestActor::onEnter() {
    PhysicsActor::onEnter();
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

    auto json = system->addJsonObject("Platformer/RubeScenes/test.json");
    if(!PhysicsActor::init(system,ActorType ::Interactive,initPosition,json)){
        return false;
    }



    //box2d stuff
    mainBody = json.getBodyByName("MainBody");
    circleBody = json.getBodyByName("CircleBody");
    joint = json.getJointByName("joint0");



//        mainBody->SetActive(false);
//        circleBody->SetActive(false);
//
//        mainBody->SetUserData(this);
//        circleBody->SetUserData(this);
//        joint->SetUserData(this);
//
//        system->addOffset(mainBody, initPosition);
//        system->addOffset(circleBody, initPosition);
//        system->addOffset(joint, initPosition);


    //cocos stuff
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

//    mainBody->SetActive(true);
//    circleBody->SetActive(true);

    setPosition(box2DToActorParentSpace(mainBody->GetPosition()));
    setRotation(AngleBToC(mainBody->GetAngle()));

    circleSprite->setPosition(box2DToActorSpace(circleBody->GetPosition()));
    circleSprite->setRotation(AngleBToC(circleBody->GetAngle()));



}

cocos2d::Vec2 TestActor2::getDeltaMovement() {
    return cocos2d::Vec2();
}


void TestActor2::postPhysicsUpdate(float delta) {



    setPosition(box2DToActorParentSpace(mainBody->GetPosition()));
    setRotation(AngleBToC(mainBody->GetAngle()));
    circleSprite->setRotation(AngleBToC(circleBody->GetAngle()));


}

