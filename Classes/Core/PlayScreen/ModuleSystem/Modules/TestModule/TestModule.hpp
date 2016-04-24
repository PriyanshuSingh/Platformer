//
// Created by ashish on 4/21/16.
//

#ifndef MYGAME_TESTMODULE_HPP
#define MYGAME_TESTMODULE_HPP

#include "../../PlayModule.hpp"
#include "../../../Physics/PhysicsActor.hpp"
class TestModule:public PlayModule {
public:
    CREATE_MODULE(TestModule);



    virtual bool init(const staticInfo &info, B2PhysicsSystem *system, MainCamera *cam, const b2Vec2 &offset) override;
    virtual void prePhysicsUpdate(float delta) override;
    virtual void postPhysicsUpdate(float delta) override;

protected:
    virtual void onCoordsStable() override;




private:

    TestActor * actor = nullptr;
    TestActor *newActor = nullptr;
    TestActor2 * actor2 = nullptr;
};


#endif //MYGAME_TESTMODULE_HPP
