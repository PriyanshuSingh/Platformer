//
// Created by ashish on 4/21/16.
//

#include "../../../PlatformerGlobals.hpp"
#include "TestModule.hpp"



USING_NS_CC;



bool TestModule::init(const PlayModule::ModuleInfo &info, B2PhysicsSystem *system, MainCamera *cam,
                      const b2Vec2 &offset) {
    if(!PlayModule::init(info, system, cam, offset)){
        return false;
    }





    PlatformerGlobals::printVec2("the offset is ",BtoC(boxInitOffset));





    return true;
}


void TestModule::prePhysicsUpdate(float delta) {
    PlayModule::prePhysicsUpdate(delta);
}

void TestModule::postPhysicsUpdate(float delta) {
    PlayModule::postPhysicsUpdate(delta);





        newActor->update(delta);
        actor2->update(delta);
}


void TestModule::onCoordsStable() {
    PlayModule::onCoordsStable();


    cocos2d::log("on coords stable");



    newActor = TestActor::create(system,this,boxInitOffset+b2Vec2(2,2));
    this->addChild(newActor);


    actor2 = TestActor2::create(system,this,boxInitOffset+b2Vec2(2,2));
    this->addChild(actor2);



}

