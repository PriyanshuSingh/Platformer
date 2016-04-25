//
// Created by ashish on 4/21/16.
//

#include "../../../PlatformerGlobals.hpp"
#include "TestModule.hpp"



USING_NS_CC;



bool TestModule::init(const PlayModule::staticInfo &info, B2PhysicsSystem *system, MainCamera *cam,
                      const b2Vec2 &offset) {
    return PlayModule::init(info, system, cam, offset);



}

void TestModule::onCoordsStable() {
    PlayModule::onCoordsStable();

#ifdef DEBUGGING_APP
    cocos2d::log("on coords stable");
#endif


    newActor = TestActor::create(system,boxInitOffset+b2Vec2(18,10));
    this->addChild(newActor);


    actor2 = TestActor2::create(system,boxInitOffset+b2Vec2(15,10));
    this->addChild(actor2);



    blocker = Blocker::create(system,boxInitOffset);
    this->addChild(blocker);

}

void TestModule::prePhysicsUpdate(float delta) {
    PlayModule::prePhysicsUpdate(delta);

    newActor->prePhysicsUpdate(delta);
    actor2->prePhysicsUpdate(delta);
    blocker->prePhysicsUpdate(delta);


}

void TestModule::postPhysicsUpdate(float delta) {
    PlayModule::postPhysicsUpdate(delta);

        newActor->postPhysicsUpdate(delta);
        actor2->postPhysicsUpdate(delta);
        blocker->postPhysicsUpdate(delta);
}


void TestModule::BeginContact(b2Contact *contact) {
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto actor1 = static_cast<PhysicsActor*>(fixA->GetBody()->GetUserData());
    auto actor2 = static_cast<PhysicsActor*>(fixB->GetBody()->GetUserData());

    if(actor1!= nullptr  && actor1 == (PhysicsActor*)blocker){

        blocker->setAnyThingInside(fixA);
        return;
    }
    if(actor2!= nullptr &&  actor2 == (PhysicsActor*)blocker){

        blocker->setAnyThingInside(fixB);
    }

}

void TestModule::EndContact(b2Contact *contact) {
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto actor1 = static_cast<PhysicsActor*>(fixA->GetBody()->GetUserData());
    auto actor2 = static_cast<PhysicsActor*>(fixB->GetBody()->GetUserData());


    if(actor1!= nullptr && actor1 == (PhysicsActor*)blocker){

        blocker->setAnyThingInside(fixA);
        return;
    }
    if(actor2!= nullptr && actor2 == (PhysicsActor*)blocker){

        blocker->setAnyThingInside(fixB);
    }
}


