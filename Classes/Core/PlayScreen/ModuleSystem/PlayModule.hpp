//
// Created by ashish on 4/11/16.
//

#ifndef MYGAME_PLAYMODULE_HPP
#define MYGAME_PLAYMODULE_HPP

#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "../PlatformerGlobals.hpp"

//helper macros
#define CREATE_MODULE(__TYPE__) \
static PlayModule* create(const staticInfo & info,B2PhysicsSystem * system,MainCamera * cam,const b2Vec2 & offset) \
{ \
    PlayModule *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init(info,system,cam,offset)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

class B2PhysicsSystem;
class b2Body;
class b2Joint;
class Player;
class MainCamera;

class PlayModule:public cocos2d::Layer,public b2ContactListener,public b2DestructionListener{
public:
    struct staticInfo{
        staticInfo(const std::string & rubeInfo,const std::string & cocosInfo):rubeInfo(rubeInfo),cocosInfo(cocosInfo){}
        std::string rubeInfo;
        std::string cocosInfo;
    };

    CREATE_MODULE(PlayModule);
    ADD_CHILD_MASK(cocos2d::Layer);



    virtual bool init(const staticInfo & info,B2PhysicsSystem * system,MainCamera * cam,const b2Vec2 & offset);

protected:
    //THIS IS YOUR ACTIVE constructor
    //before this your pre/post physics update are not called,
    //you don't get world callbacks,player is null etc.
    virtual void onCoordsStable();


public:
    //TODO delete all the acquired bodies and joints here
    virtual ~PlayModule();

    std::vector<b2Body*> & getAddedBodies();


    float getPlayerPositionPercentage();


//useful methods
    virtual void prePhysicsUpdate(float delta){};
    //TODO update cocos2d-x data(setPosition and stuff here)
    virtual void postPhysicsUpdate(float delta);


//TODO remove this,module self responsible for deleting
//player related methods
    virtual void onPlayerDeath(){}


//cocos2d-x overrides

//Box2D overrides

//collision callbacks
    virtual void BeginContact(b2Contact *contact){}
    virtual void EndContact(b2Contact *contact){}
    virtual void PreSolve(b2Contact *contact, const b2Manifold *oldManifold){}
    virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse){}
//destruction callbacks
    virtual void SayGoodbye(b2Joint *joint){}
    virtual void SayGoodbye(b2Fixture *fixture){}

//the start offset
    float getOffset();
    void setCoordinatesStabilized(bool stable);
    void addPlayer(Player *player);
    void removePlayer();
    const b2Vec2 & getInitOFfset(){return boxInitOffset;}
    bool getIsStable()const;

protected:


    //weak reference to player
    Player * player = nullptr;
    //weak reference to camera
    MainCamera * cam = nullptr;
    B2PhysicsSystem * system = nullptr;
    //vector of bodies and joints
    std::vector<b2Body*> bodies;


//TODO adjust the value of this guy in last module Accordingly
//so that the module ends before hitting the corner of game :) and you can play
//final animation and stuff

    float adjustOffset;
protected:
    enum DRAWORDER{
        BACKGROUND = 0,
        //the real World bojects
        MIDDLEGROUND = 10,

        PLAYER = 15,

        FOREGROUND = 20

    };



    void initBodies(const b2Vec2 &offset);




    //TODO see where to keep them
    //probably better to have them in a seperate Node in the PlayScene
    //add foreground and background objects to these guys
    cocos2d::ParallaxNode * backGroundObject;
    cocos2d::ParallaxNode * foreGroundObject;



    bool coordsStable = false;







//parent methods made private here
    using cocos2d::Layer::setAnchorPoint;
    b2Vec2 boxInitOffset;
};







#endif //MYGAME_PLAYMODULE_HPP
