//
// Created by ashish on 4/15/16.
//

#ifndef MYGAME_PLAYERCONTROLLER_HPP
#define MYGAME_PLAYERCONTROLLER_HPP


#include "cocos2d.h"
#include "PlayerCommand.hpp"


class Player;
class PlayerCommand;


class PlayerController{
public:

    virtual bool init(Player * target);
    virtual PlayerCommand* getCommand()=0;
    virtual void prePhysicsUpdate(float delta){}
    virtual void postPhysicsUpdate(float delta){}

    virtual ~PlayerController();

protected:
    Player * target = nullptr;

};








class InputController:public cocos2d::Layer, public PlayerController{
public:


    static InputController * create(Player * target);
    virtual bool init(Player * target);
    virtual ~InputController();

    PlayerCommand* getCommand()override;

    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) override;
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) override;

    void prePhysicsUpdate(float delta)override;
    void postPhysicsUpdate(float delta)override;

private:
    JumpCommand * jumper = nullptr;
    MoveCommand * mover = nullptr;
    KillCommand * killer = nullptr;
    AliveCommand * liver = nullptr;
    PlayerCommand * current = nullptr;



    bool movingLeft = false;
    bool movingRight = false;

    bool jumping = false;
    bool killing = false;
    bool aliving = false;

    using cocos2d::Layer::setAnchorPoint;
    using cocos2d::Layer::setPosition;
    using cocos2d::Layer::setCameraMask;

};



class TestAiController:public cocos2d::Layer, public PlayerController{
public:


    static TestAiController * create(Player * target);
    virtual bool init(Player * target);
    virtual ~TestAiController();

    PlayerCommand* getCommand()override;



    void prePhysicsUpdate(float delta)override;

private:
    MoveCommand * mover = nullptr;
    PlayerCommand * current = nullptr;


    bool moving = false;
    bool touchCame = false;

    using cocos2d::Layer::setAnchorPoint;
    using cocos2d::Layer::setPosition;
    using cocos2d::Layer::setCameraMask;


};










#endif //MYGAME_PLAYERCONTROLLER_HPP
