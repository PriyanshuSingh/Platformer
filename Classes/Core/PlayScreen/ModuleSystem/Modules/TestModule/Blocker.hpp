//
// Created by ashish on 4/24/16.
//

#ifndef MYGAME_BLOCKER_HPP
#define MYGAME_BLOCKER_HPP

#include "../../../Physics/PhysicsActor.hpp"

class Blocker :public PhysicsActor{


public:
    static Blocker* create(B2PhysicsSystem * system,const b2Vec2 & initPosition);

    bool init(B2PhysicsSystem *system, const b2Vec2 &initPosition);

    virtual void onEnter() override;

    virtual ~Blocker(){};

    virtual cocos2d::Vec2 getDeltaMovement() override;

    virtual void prePhysicsUpdate(float delta) override;

    virtual void postPhysicsUpdate(float delta) override;


    void setAnyThingInside(b2Fixture * fix){
        if(fix == sensorFix){
            anyThingInside = !anyThingInside;
        }
    }
private:
    float maxDisplacement = 4.5;
    float timeToUp = 6;
    float timeToDown = 3;


    b2Fixture * sensorFix = nullptr;
    b2Body * blocker = nullptr;
    bool anyThingInside = false;


    b2Vec2 startPosition;
};


#endif //MYGAME_BLOCKER_HPP
