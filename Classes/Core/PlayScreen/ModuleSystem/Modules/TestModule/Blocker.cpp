//
// Created by ashish on 4/24/16.
//

#include "Blocker.hpp"
#include "../../../Physics/b2PhysicsSystem.hpp"


Blocker *Blocker::create(B2PhysicsSystem *system, const b2Vec2 &initPosition) {
    auto bActor = new(std::nothrow)Blocker();
    if(bActor && bActor->init(system,initPosition)){
        bActor->autorelease();
        return bActor;
    }
    CC_SAFE_DELETE(bActor);
    return nullptr;
}

bool Blocker::init(B2PhysicsSystem *system, const b2Vec2 &initPosition) {

    auto json = system->addJsonObject("Platformer/Modules/TestModule/Dynamic/Blocker.json");

    if(!PhysicsActor::init(system,ActorType ::Interactive, initPosition,json)){
        return false;
    }

    {
        blocker = json.getBodyByName("Blocker");
        sensorFix = json.getFixtureByName("b");
        startPosition = blocker->GetPosition();
    }



//    blocker->SetActive(false);
//    sensorFix->GetBody()->SetActive(false);

//    blocker->SetUserData(this);
//    sensorFix->GetBody()->SetUserData(this);

//    system->addOffset(blocker, initPosition);
//    system->addOffset(sensorFix->GetBody(), initPosition);

    return true;


}

void Blocker::onEnter() {
    PhysicsActor::onEnter();




}

cocos2d::Vec2 Blocker::getDeltaMovement() {
    return cocos2d::Vec2();
}

void Blocker::prePhysicsUpdate(float delta) {


    auto vel = blocker->GetLinearVelocity();
    auto position = blocker->GetPosition();
    if(numThingsInside>0){
        vel.y = (position.y-startPosition.y >= maxDisplacement)?0:maxDisplacement/timeToUp;
    }
    else{
        vel.y = (position.y-startPosition.y <= 0)?0:-maxDisplacement/timeToDown;
    }
    blocker->SetLinearVelocity(vel);

}

void Blocker::postPhysicsUpdate(float delta) {


}

