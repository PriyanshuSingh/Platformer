//
// Created by ashish on 4/11/16.
//

#ifndef MYGAME_PLAYER_HPP
#define MYGAME_PLAYER_HPP


#include "../Physics/PhysicsActor.hpp"

class b2Body;
class B2PhysicsSystem;
class ModuleContainer;
class PlayModule;
class PlayerController;
class PlayerCommand;


class Player: public PhysicsActor{

    friend class JumpCommand;
    friend class MoveCommand;
    friend class KillCommand;
    friend class AliveCommand;

public:
    //creation deletion methods
    static Player * create(ModuleContainer * container,B2PhysicsSystem * system,const b2Vec2 & initPosition);
    bool init(ModuleContainer * container,B2PhysicsSystem *system,const b2Vec2 & initPosition);
    //TODO add reinit method for death control
    //bool reinit(){calls parent init with keep body acitve }

    bool isGrounded(){return grounded;}
    bool isAttached(){return attached;}





//TODO stuff

//replace with commands
//think design of these methods
//useful when the module wants to change player behaviour
    void clearPhysicsStuff(){}
    void initPhysicsInfo(){}

//TODO
//add interface for grabbing,walking/running,Climbing
//player states




//Node overrides
    virtual void setParent(cocos2d::Node * parent)override;


//Physics Actor overrides
    cocos2d::Vec2 getDeltaMovement()override ;
    void prePhysicsUpdate(float delta)override;
    void postPhysicsUpdate(float delta)override;
    virtual void onEnter() override;

    void addController(PlayerController * controller);
    void removeController(PlayerController * controller);


private:
    //player state
    cocos2d::Vec2 playerDeltaMovement;
    bool grounded = false;
    bool attached = false;
    cocos2d::Vec2 prevPosition;
    bool dead = false;
    //player physics components
    b2Body *mainBody = nullptr;
    b2Fixture * groundFixture = nullptr;


    std::vector<PlayerController *> controllers;





//Non state vars
private:

    //weak reference to module currently in
    PlayModule * parentModule = nullptr;
    //weak reference to Module
    ModuleContainer * container = nullptr;




//Methods
private:
    using cocos2d::Node::setAnchorPoint;
    void killMe();
    void bringToLife(const b2Vec2 &pos);
    cocos2d::Sprite *playerSprite;
    void setupPlayerSprite(b2dJson *json);

};


#endif //MYGAME_PLAYER_HPP
