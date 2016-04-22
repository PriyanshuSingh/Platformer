//
// Created by ashish on 4/21/16.
//

#include <Core/PlayScreen/PlatformerGlobals.hpp>
#include "TestModule.hpp"



USING_NS_CC;
PlayModule *TestModule::create(const PlayModule::ModuleInfo &info, B2PhysicsSystem *system, MainCamera *cam,
                               const b2Vec2 &offset) {




    auto module = new (std::nothrow) TestModule();
    if (module && module->init(info,system,cam,offset))
    {
        module->autorelease();
        return module;
    }
    CC_SAFE_DELETE(module);
    return nullptr;



}


bool TestModule::init(const PlayModule::ModuleInfo &info, B2PhysicsSystem *system, MainCamera *cam,
                      const b2Vec2 &offset) {
    if(!PlayModule::init(info, system, cam, offset)){
        return false;
    }




    PlatformerGlobals::printVec2("the offset is ",BtoC(boxInitOffset));



    bool static once  = true;
    if(once){
        once = false;

        srand(time(NULL));

    }


    return true;
}


void TestModule::prePhysicsUpdate(float delta) {
    PlayModule::prePhysicsUpdate(delta);
}

void TestModule::postPhysicsUpdate(float delta) {
    PlayModule::postPhysicsUpdate(delta);

    if(newActor!= nullptr){
        newActor->postPhysicsUpdate(delta);
    }
}


void TestModule::onCoordsStable() {
    PlayModule::onCoordsStable();


    cocos2d::log("on coords stable");




    newActor = TestActor::create(system,this,boxInitOffset);
    this->addChild(newActor);
    newActor->onModuleActive();



}

