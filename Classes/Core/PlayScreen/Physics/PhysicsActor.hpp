//
// Created by ashish on 4/12/16.
//

#ifndef MYGAME_GAMEACTOR_HPP
#define MYGAME_GAMEACTOR_HPP

#include "rubeStuff/b2dJson.h"
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "../PlatformerGlobals.hpp"

class B2PhysicsSystem;
class b2Body;
class PlayModule;
//a physics Driven Object
class PhysicsActor:public cocos2d::Node {
public:

    /*Use tag to break ties between objects which have same Actor Type*/
    const static int DontCareTag = -1;
    const static int MainPlayerTag = 0;

    enum class ActorType{
        Player,
        Void,
        Interactive,
        Static

    };
    ADD_CHILD_MASK(cocos2d::Node);

    typedef std::vector<b2Body*> bVec;
    typedef std::vector<b2Joint*> jVec;




    /*By defualt activates all the bodies
     * override this method to use setup initial position of cocos2d-x objects
     * based on Box2d objects as this is called
     * when added to the "Stage"*/

    virtual void onEnter() override;
    /*The Destructor deletes all the physics
     * bodies and joints so subclasses shouldn't Do So!!
     */

    virtual ~PhysicsActor();
    //return movement in Cocos Coordinates based on your physics Stuff
    virtual cocos2d::Vec2 getDeltaMovement()=0;
    //conversion helpers from box2d world to cocos
    cocos2d::Vec2 box2DToActorSpace(const b2Vec2 & bPos)const;
    cocos2d::Vec2 box2DToActorParentSpace(const b2Vec2 bPos)const;





    //TODO resort back to prephysics and post Physics pattern :(
    virtual void prePhysicsUpdate(float delta){}
    virtual void postPhysicsUpdate(float delta){}





    //default behaviour is to set gravity scale
    //on each body but you can override this
    //if you are interested in particular bodies Only
//    virtual void setGravityScale(float scale);


protected:

    bVec & getAllBodies(){return bodies;}
    jVec & getAlljoints(){return joints;}

    void setupPhysicsObjects(const b2Vec2 & pos,bool active);
    void deleteAllBodiesAndJoints();



    bool init(B2PhysicsSystem * system,const ActorType & type,const b2Vec2 & initPosition,const bVec & bodies,const jVec & joints);
    bool init(B2PhysicsSystem * system,const ActorType & type,const b2Vec2 & initPosition,b2dJson & json);

    //by default sets user data of all the bodies and joints == this
    virtual void setUserData();
    //by default set all bodies and joints passed at init position
    virtual void setAbsPosition(const b2Vec2 & pos);
    //by default add offset to all bodies and joints passed are set Active/Inactive
    virtual void setBodiesActive(bool flag);

    B2PhysicsSystem * system = nullptr;
    ActorType type = ActorType::Void;



private:
    bool commonInit(B2PhysicsSystem *system,const ActorType & type,const b2Vec2 & initPosition);

    bVec bodies;
    jVec joints;

};


class TestActor:public PhysicsActor{
public:

    static TestActor * create(B2PhysicsSystem * system,const b2Vec2 & initPosition);
    bool init(B2PhysicsSystem *system,const b2Vec2 & initPosition);

    void onEnter() override;
    virtual ~TestActor(){};

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
    virtual ~TestActor2(){};

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
