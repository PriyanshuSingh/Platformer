//
// Created by ashish on 4/21/16.
//

#include "../../../PlatformerGlobals.hpp"
#include "TestModule.hpp"
#include "../../../Physics/b2PhysicsSystem.hpp"


USING_NS_CC;



bool TestModule::init(const PlayModule::staticInfo &info, B2PhysicsSystem *system, MainCamera *cam,
                      const b2Vec2 &offset) {
    if(!PlayModule::init(info, system, cam, offset)){
        return false;
    }

    auto keyListener = EventListenerKeyboard::create();

    keyListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){

        if(this->coordsStable) {
            switch (keyCode) {
                case EventKeyboard::KeyCode::KEY_R:

                    switcher *=-1;
                    break;
                case EventKeyboard::KeyCode::KEY_P:
                    static bool switcher = true;
                    if(switcher){
                        system->pause();
                    }
                    else {
                        system->resume();
                    }
                    switcher = !switcher;
                    break;
                default:
                    break;
            }


        }
    };
    keyListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){


    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener,this);

    return true;




}

void TestModule::onCoordsStable() {
    PlayModule::onCoordsStable();

#ifdef DEBUGGING_APP
    cocos2d::log("on coords stable");
#endif


    newActor = TestActor::create(system,boxInitOffset+b2Vec2(2,10));
    this->addChild(newActor);


    actor2 = TestActor2::create(system,boxInitOffset+b2Vec2(13,12));
    this->addChild(actor2);



    blocker = Blocker::create(system,boxInitOffset);
    this->addChild(blocker);

}

void TestModule::prePhysicsUpdate(float delta) {

    PlayModule::prePhysicsUpdate(delta);

    newActor->setGravityScale(switcher);
    //TODO just for testing purposes
//    actor2->setGravityScale(switcher);
    blocker->setGravityScale(switcher);

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
    PhysicsActor* actor1 = static_cast<PhysicsActor*>(fixA->GetBody()->GetUserData());
    PhysicsActor* actor2 = static_cast<PhysicsActor*>(fixB->GetBody()->GetUserData());

//    cocos2d::log("blocker is %p",static_cast<PhysicsActor*>(blocker));
//    cocos2d::log("actor1 is %p",actor1);
//    cocos2d::log("actor2 is %p",actor2);


    if((actor1!= nullptr)  && (actor1 == static_cast<PhysicsActor*>(blocker))){


            blocker->setAnyThingInside(fixA,true);



        return;
    }
    if((actor2!= nullptr)  && (actor2 == static_cast<PhysicsActor*>(blocker))) {
        blocker->setAnyThingInside(fixB,true);

    }
}

void TestModule::EndContact(b2Contact *contact) {
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto actor1 = static_cast<PhysicsActor*>(fixA->GetBody()->GetUserData());
    auto actor2 = static_cast<PhysicsActor*>(fixB->GetBody()->GetUserData());


    if(actor1!= nullptr && actor1 == (PhysicsActor*)blocker){

        blocker->setAnyThingInside(fixA,false);

        return;
    }
    if(actor2!= nullptr && actor2 == (PhysicsActor*)blocker){


        blocker->setAnyThingInside(fixB,false);

    }
}


