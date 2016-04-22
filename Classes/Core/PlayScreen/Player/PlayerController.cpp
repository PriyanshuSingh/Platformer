//
// Created by ashish on 4/15/16.
//

#include "PlayerController.hpp"
#include "Player.hpp"
USING_NS_CC;
/*
 * Player Controller
 */
bool PlayerController::init(Player * target) {
    this->target = target;
    return true;
}

PlayerController::~PlayerController() {

    if(target!= nullptr){
        target->removeController(this);
    }


}

/*
 * Input Controller
 */


InputController *InputController::create(Player *target) {
    auto iC = new(std::nothrow)InputController();
    if (iC && iC->init(target) ) {
        iC->autorelease();
        return iC;
    }
    CC_SAFE_DELETE(iC);
    return nullptr;
}


bool InputController::init(Player *target) {
    if(!Layer::init() || !PlayerController::init(target))return false;

    {
        ignoreAnchorPointForPosition(false);
        setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        setPosition(Vec2::ZERO);
        //TODO add children(virtual joystick and stuff such that
        //they part of visible screen(use csloader add visible);
        setCameraMask((unsigned short)CameraFlag::DEFAULT,true);
    }


    target->addController(this);

    jumper = JumpCommand::create(target);
    jumper->retain();

    mover = MoveCommand::create(target,1.0);
    mover->retain();


    killer = KillCommand::create(target);
    killer->retain();


    liver = AliveCommand::create(target,b2Vec2(10.000040, 0.039395),0);
    liver->retain();

    auto eventKeyboard = EventListenerKeyboard::create();
    eventKeyboard->onKeyPressed = CC_CALLBACK_2(InputController::onKeyPressed,this);
    eventKeyboard->onKeyReleased = CC_CALLBACK_2(InputController::onKeyReleased,this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventKeyboard,this);

    return true;

}

InputController::~InputController() {

    CC_SAFE_RELEASE(jumper);
    CC_SAFE_RELEASE(mover);
    CC_SAFE_RELEASE(killer);
    CC_SAFE_RELEASE(liver);

}

PlayerCommand *InputController::getCommand() {
    return current;
}



void InputController::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {



    if(keyCode == EventKeyboard::KeyCode::KEY_SPACE){
        jumping = true;

    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW){
        mover->setDirection(true);
        movingRight = true;

    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW){
        mover->setDirection(false);
        movingLeft = true;

    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_D){
        killing = true;

    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_L){

        aliving = true;

    }





}

void InputController::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {


    typedef EventKeyboard::KeyCode Codes;
    if(keyCode == Codes::KEY_SPACE){
        jumping = false;

    }
    else if(keyCode == Codes::KEY_RIGHT_ARROW){
        movingRight = false;
    }
    else if(keyCode == Codes::KEY_LEFT_ARROW){
        movingLeft = false;
    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_D){
        killing = false;

    }
    else if(keyCode == EventKeyboard::KeyCode::KEY_L){
        aliving = false;

    }


}


void InputController::prePhysicsUpdate(float delta) {

    //give priority to aliving command



    if(aliving){
        current = liver;
        aliving = false;
        return;
    }
    if(killing){
        current = killer;
        killing = false;
        return;
    }

    if(jumping){
        jumping = false;
        current = jumper;
        return;
    }

    if(movingLeft || movingRight){
        current = mover;
        return;
    }


    current = nullptr;

}

void InputController::postPhysicsUpdate(float delta) {

}


TestAiController *TestAiController::create(Player *target) {
    auto ai = new(std::nothrow)TestAiController();
    if (ai && ai->init(target) ) {
        ai->autorelease();
        return ai;
    }
    CC_SAFE_DELETE(ai);
    return nullptr;
}

bool TestAiController::init(Player *target) {
    if(!Layer::init() || !PlayerController::init(target))return false;


    target->addController(this);


    mover = MoveCommand::create(target,0.0f);
    mover->retain();


    current = mover;

    return true;
}

TestAiController::~TestAiController() {

    CC_SAFE_RELEASE(mover);
}

PlayerCommand *TestAiController::getCommand() {
    return nullptr;
}

void TestAiController::prePhysicsUpdate(float delta) {
    CC_UNUSED_PARAM(delta);

}

