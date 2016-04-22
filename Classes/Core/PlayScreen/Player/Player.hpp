//
// Created by ashish on 4/11/16.
//

#ifndef MYGAME_PLAYER_HPP
#define MYGAME_PLAYER_HPP


#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "../../../rubeStuff/b2dJson.h"
class b2Body;
class B2PhysicsSystem;
class ModuleContainer;
class PlayModule;
class PlayerController;
class PlayerCommand;


class Player: public cocos2d::Node{

    friend class JumpCommand;
    friend class MoveCommand;
    friend class KillCommand;
    friend class AliveCommand;

public:
    //creation deletion methods
    static Player * create(ModuleContainer * container,B2PhysicsSystem * system);
    bool init(ModuleContainer * container,B2PhysicsSystem *system);

    //TODO check constraints like player velocity here
    void prePhysicsUpdate(float delta);
    //TODO update cocos2d-x data(setPosition and stuff here)
    void postPhysicsUpdate(float delta);

    //TODO replace with commands






    //TODO think design of these methods
    //useful when the module wants to change player behaviour
    void clearPhysicsStuff();
    void initPhysicsInfo();


    //TODO bodies deletion
    virtual ~Player();



    virtual void setParent(cocos2d::Node * parent)override;

    //TODO
    //add identification for collision callbacks
    //also use the
    //add interface for grabbing,walking/running,Climbing
    //player states




//TODO add priority based controllers here



//TODO implement this method!!!!!!!!!!!
//for syncing sprites with physics bodies
    void syncPositionWithPhysics();
//distance moved in one logical physics step
    cocos2d::Vec2 getDeltaMovement();





    bool isGrounded(){return grounded;}
    bool isAttached(){return attached;}






    void addController(PlayerController * controller);
    void removeController(PlayerController * controller);


//player state
private:
    //player displacement since last update
    cocos2d::Vec2 playerDeltaMovement;
    bool grounded;
    bool attached;

    std::vector<PlayerController *> controllers;





//save player body info
    struct PlayerInfo {
        std::string rubeInfo;
        Json::Value mainBody;

    };

    PlayerInfo playerInfo;

//required stuff
private:

    //weak reference to physics system
    B2PhysicsSystem * system = nullptr;
    //weak reference to module currently in
    PlayModule * parentModule = nullptr;
    //weak reference to Module
    ModuleContainer * container = nullptr;

    //player physics components
    b2Body *mainBody = nullptr;
    b2Fixture * groundFixture = nullptr;


    bool dead = false;



private:
    using cocos2d::Node::setAnchorPoint;
    void saveState();
    void killMe();
    void bringToLife(const b2Vec2 &pos,float degrees);

    b2Vec2 prevPosition;
};


#endif //MYGAME_PLAYER_HPP
