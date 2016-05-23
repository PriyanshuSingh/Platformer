//
// Created by ashish on 4/21/16.
//

#ifndef MYGAME_TESTMODULE_HPP
#define MYGAME_TESTMODULE_HPP

#include "../../PlayModule.hpp"
#include "../../../Physics/PhysicsActor.hpp"
#include "Blocker.hpp"

class TestModule:public PlayModule {
public:
    CREATE_MODULE(TestModule)

    virtual void BeginContact(b2Contact *contact) override;
    virtual void EndContact(b2Contact *contact) override;


    virtual bool init(const staticInfo &info,B2PhysicsSystem * system,MainCamera *cam, const b2Vec2 &offset) override;
    virtual void preUpdate(float delta)override;
    virtual void postUpdate(float delta)override;

protected:




private:

    virtual void onCoordsStable() override;
    PhysicsActor *newActor = nullptr;
    PhysicsActor * actor2 = nullptr;
    Blocker * blocker = nullptr;
    float switcher = 1;
};


#endif //MYGAME_TESTMODULE_HPP
