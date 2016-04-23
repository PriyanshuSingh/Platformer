//
// Created by ashish on 4/12/16.
//

#ifndef MYGAME_GAMEACTOR_HPP
#define MYGAME_GAMEACTOR_HPP

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "../PlatformerGlobals.hpp"

class B2PhysicsSystem;
class b2Body;
class PlayModule;
//a physics Driven Object
class PhysicsActor:public cocos2d::Node {
public:
    enum class ActorType{
        Player,
        Void,
        Interactive,
        Static

    };
    ADD_CHILD_MASK(cocos2d::Node);
    virtual bool init(B2PhysicsSystem * system,PlayModule * parent,const b2Vec2 & initOffset);
    virtual ~PhysicsActor(){}
    virtual void prePhysicsUpdate(float delta){}
    virtual void postPhysicsUpdate(float delta){}
    //return movement in Cocos Coordinates based on your physics Stuff
    virtual cocos2d::Vec2 getDeltaMovement()=0;
    void setModuleActive(bool stable);





protected:
    B2PhysicsSystem * system = nullptr;
    PlayModule * parentModule = nullptr;
    ActorType type = ActorType::Void;
private:
    virtual void onModuleActive()=0;
    bool stable = false;

};


class TestActor:public PhysicsActor{
public:

    static TestActor * create(B2PhysicsSystem * system,PlayModule * parent,const b2Vec2 & initOffset);
    bool init(B2PhysicsSystem *system,PlayModule * parent,const b2Vec2 & initOffset) override;
    virtual ~TestActor();
    virtual void postPhysicsUpdate(float delta) override;

    cocos2d::Vec2 getDeltaMovement() override;


private:
    void onModuleActive()override;
    b2Body * bod = nullptr;
    cocos2d::Sprite *sprite = nullptr;
};






#endif //MYGAME_GAMEACTOR_HPP
