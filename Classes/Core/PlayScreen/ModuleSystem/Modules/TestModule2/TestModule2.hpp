//
// Created by Priyanshu Singh on 03/05/16.
//

#ifndef PLATFORMER_TESTMODULE2_HPP
#define PLATFORMER_TESTMODULE2_HPP


#include "../../PlayModule.hpp"
#include "../../../Physics/PhysicsActor.hpp"
#include "../../../Physics/Box2dLights/Lights/PointLight.hpp"

class TestModule2: public PlayModule {
public:
    CREATE_MODULE(TestModule2)

    virtual void onEnter() override;
    virtual void BeginContact(b2Contact *contact) override;
    virtual void EndContact(b2Contact *contact) override;

    virtual bool init(const staticInfo &info,B2PhysicsSystem * system,MainCamera *cam, const b2Vec2 &offset) override;
    void preUpdate(float delta) override;
    void postUpdate(float delta) override;

protected:

    box2dLight::LightHandler *lightHandler;
    box2dLight::PointLight *pointLight;

private:
    void onCoordsStable() override;
};



#endif //PLATFORMER_TESTMODULE2_HPP
