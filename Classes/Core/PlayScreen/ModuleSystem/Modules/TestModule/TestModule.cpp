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
    //add static data here
    //load the rube stuff
    //load the Csloader stuff:)




    auto back = Sprite::create("groundalpha1.png");
    back->setAnchorPoint(Vec2::ZERO);
    this->addChild(back);








    return true;
}

void TestModule::onCoordsStable() {
    PlayModule::onCoordsStable();

#ifdef DEBUGGING_APP
    cocos2d::log("on coords stable");
#endif


    newActor = TestActor::create(system,boxInitOffset+b2Vec2(6,10));
    this->addChild(newActor);


    actor2 = TestActor2::create(system,boxInitOffset+b2Vec2(6,10));
    this->addChild(actor2);



}

void TestModule::prePhysicsUpdate(float delta) {
    PlayModule::prePhysicsUpdate(delta);

    newActor->prePhysicsUpdate(delta);
    actor2->prePhysicsUpdate(delta);


}

void TestModule::postPhysicsUpdate(float delta) {
    PlayModule::postPhysicsUpdate(delta);





        newActor->postPhysicsUpdate(delta);
        actor2->postPhysicsUpdate(delta);



}



