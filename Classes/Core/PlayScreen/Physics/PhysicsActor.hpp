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
    virtual bool init(B2PhysicsSystem * system,const ActorType & type,const b2Vec2 & initPosition);


    //override this method to use setup initial position based
    //on Box2d objects as this is called when added to the "Stage"
    void onEnter() override;
    virtual ~PhysicsActor(){}
    //return movement in Cocos Coordinates based on your physics Stuff
    virtual cocos2d::Vec2 getDeltaMovement()=0;
    cocos2d::Vec2 box2DToActorSpace(const b2Vec2 & bPos)const;
    cocos2d::Vec2 box2DToActorParentSpace(const b2Vec2 bPos)const;





    //TODO resort back to prephysics and post Physics pattern :(
    virtual void prePhysicsUpdate(float delta){}
    virtual void postPhysicsUpdate(float delta){}


protected:
    B2PhysicsSystem * system = nullptr;
    b2Vec2 initPosition;
    ActorType type = ActorType::Void;
private:

};


class TestActor:public PhysicsActor{
public:

    static TestActor * create(B2PhysicsSystem * system,const b2Vec2 & initPosition);
    bool init(B2PhysicsSystem *system,const b2Vec2 & initPosition);

    void onEnter() override;
    virtual ~TestActor();

    cocos2d::Vec2 getDeltaMovement() override;


    virtual void postPhysicsUpdate(float delta) override;

private:

    b2Body * bod = nullptr;
    cocos2d::Sprite * sprite = nullptr;
};


class TestActor2:public PhysicsActor{
public:

    static TestActor2 * create(B2PhysicsSystem * system,const b2Vec2 & initPosition);
    bool init(B2PhysicsSystem *system,const b2Vec2 & initPosition);

    void onEnter() override;
    virtual ~TestActor2();

    cocos2d::Vec2 getDeltaMovement() override;

    virtual void postPhysicsUpdate(float delta) override;

private:

    cocos2d::Sprite *circleSprite = nullptr;
    cocos2d::Sprite *boxSprite = nullptr;


    b2Body *mainBody = nullptr;
    b2Body *circleBody = nullptr;
    b2Joint * joint = nullptr;


};








#endif //MYGAME_GAMEACTOR_HPP
