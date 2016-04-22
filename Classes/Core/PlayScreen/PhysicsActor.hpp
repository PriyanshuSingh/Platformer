//
// Created by ashish on 4/12/16.
//

#ifndef MYGAME_GAMEACTOR_HPP
#define MYGAME_GAMEACTOR_HPP

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class B2PhysicsSystem;
class b2Body;
class PlayModule;
//a physics Driven Object
class PhysicsActor:public cocos2d::Node {
public:
    enum class ActorType{
        Player,
        Void

    };


    virtual void addChild(cocos2d::Node *child) override {
        cocos2d::Node::addChild(child);
        child->setCameraMask(getCameraMask());
    }

    virtual void addChild(cocos2d::Node *child, int localZOrder) override {
        cocos2d::Node::addChild(child, localZOrder);
        child->setCameraMask(getCameraMask());
    }

    virtual void addChild(cocos2d::Node *child, int localZOrder, int tag) override {
        cocos2d::Node::addChild(child, localZOrder, tag);
        child->setCameraMask(getCameraMask());
    }

    virtual void addChild(cocos2d::Node *child, int localZOrder, const std::string &name) override {
        cocos2d::Node::addChild(child, localZOrder, name);
        child->setCameraMask(getCameraMask());
    }

    virtual bool init(B2PhysicsSystem * system,PlayModule * parent,const b2Vec2 & offset);
    virtual ~PhysicsActor(){}
    virtual void prePhysicsUpdate(float delta){}
    virtual void postPhysicsUpdate(float delta){}
    //return movement in Cocos Coordinates based on your physics Stuff
    virtual cocos2d::Vec2 getDeltaMovement()=0;

protected:
    B2PhysicsSystem * system = nullptr;
    PlayModule * parentModule = nullptr;
    b2Vec2 offset;
    ActorType type = ActorType::Void;
private:

};


class TestActor:public PhysicsActor{
public:
    virtual void postPhysicsUpdate(float delta) override;
    static TestActor * create(B2PhysicsSystem * system,PlayModule * parent,const b2Vec2 & offset);
    void onModuleActive();

    bool init(B2PhysicsSystem *system,PlayModule * parent,const b2Vec2 & offset) override;
    cocos2d::Vec2 getDeltaMovement() override;


private:
    b2Body * bod = nullptr;


    cocos2d::Sprite *sprite = nullptr;
};

#endif //MYGAME_GAMEACTOR_HPP
