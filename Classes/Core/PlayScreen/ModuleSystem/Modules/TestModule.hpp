//
// Created by ashish on 4/21/16.
//

#ifndef MYGAME_TESTMODULE_HPP
#define MYGAME_TESTMODULE_HPP

#include "../PlayModule.hpp"
#include "../../PhysicsActor.hpp"
class TestModule:public PlayModule {
public:
    CREATE_MODULE(TestModule);


private:
    virtual void onCoordsStable() override;

    virtual void prePhysicsUpdate(float delta) override;

    virtual void postPhysicsUpdate(float delta) override;

    virtual bool init(const ModuleInfo &info, B2PhysicsSystem *system, MainCamera *cam, const b2Vec2 &offset) override;



private:

    TestActor * actor = nullptr;

    TestActor *newActor = nullptr;
};


#endif //MYGAME_TESTMODULE_HPP
