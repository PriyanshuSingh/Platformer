//
// Created by ashish on 4/11/16.
//

#ifndef MYGAME_PLAYMODULE_HPP
#define MYGAME_PLAYMODULE_HPP

#include "Box2D/Box2D.h"
#include "cocos2d.h"


class B2PhysicsSystem;
class b2Body;
class b2Joint;
class Player;
class MainCamera;

class PlayModule:public cocos2d::Layer,public b2ContactListener,public b2DestructionListener{
public:

    struct ModuleInfo{
        ModuleInfo(const std::string & rubeInfo):rubeInfo(rubeInfo){}
        std::string rubeInfo;
    };


    static PlayModule * create(const ModuleInfo & info, B2PhysicsSystem * system,MainCamera * cam,const b2Vec2 & offset);
    virtual bool init(const ModuleInfo & info,B2PhysicsSystem * system,MainCamera * cam,const b2Vec2 & offset);
    //TODO delete all the acquired bodies and joints here
    virtual ~PlayModule();

    std::vector<b2Body*> & getAddedBodies();
    std::vector<b2Joint*>& getAddedJoints();

    float getPlayerPositionPercentage();


//useful methods
    virtual void prePhysicsUpdate(float delta){};
    //TODO update cocos2d-x data(setPosition and stuff here)
    virtual void postPhysicsUpdate(float delta);


//TODO remove this,module self responsible for deleting
//player related methods
    virtual void onPlayerDeath(){}



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
    const cocos2d::Mat4 & getbox2dToModuleMatrix(){ return box2dToModuleMatrix;}
    const b2Vec2 & getInitOFfset(){return boxInitOffset;}
    cocos2d::Vec2 tranformBox2DToModule(const b2Vec2 & pos);
protected:


    //weak reference to player
    Player * player = nullptr;
    //weak reference to camera
    MainCamera * cam = nullptr;
    B2PhysicsSystem * system = nullptr;
    //vector of bodies and joints
    std::vector<b2Body*> bodies;
    std::vector<b2Joint*> joints;

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



    void addOffsetBodiesAndJoints(const b2Vec2 &offset);




    //TODO see where to keep them
    //probably better to have them in a seperate Node in the PlayScene
    //add foreground and background objects to these guys
    cocos2d::ParallaxNode * backGroundObject;
    cocos2d::ParallaxNode * foreGroundObject;



    bool coordsStable = false;





    cocos2d::Mat4 box2dToModuleMatrix;


protected:
    virtual void onCoordsStable();

//parent methods made private here
    using cocos2d::Layer::setAnchorPoint;


    b2Vec2 boxInitOffset;
};


#endif //MYGAME_PLAYMODULE_HPP
