//
// Created by ashish on 4/11/16.
//

#ifndef MYGAME_MODULECONTAINER_HPP
#define MYGAME_MODULECONTAINER_HPP

#include "cocos2d.h"
#include "../PlatformerGlobals.hpp"
#include "PlayModule.hpp"

class B2PhysicsSystem;
class Player;
class MainCamera;
class InputController;
class TestAiController;

class ModuleContainer :public cocos2d::Node {
public:
    static ModuleContainer * create(B2PhysicsSystem * system,MainCamera * cam);
    bool init(B2PhysicsSystem * system,MainCamera * cam);
    ~ModuleContainer();

    CC_DEPRECATED_ATTRIBUTE void move(float xDisplacement,float zoomFactor);


//public API
    void onEnterLastModule();
    //TODO remove this and use a interface instead like for Player controllers
    void addSwitchCurrentCallback(const  std::function<void()> & callback);
    PlayModule *getCurrentModule();


    void prePhysicsUpdate(float delta);
    void postPhysicsUpdate(float delta);


private:
    enum DRAWORDER{
        BACKGROUND = 0,
        MIDDLEGROUND = 10,
        FOREGROUND = 20
    };



    //weak references
    B2PhysicsSystem * system = nullptr;
    MainCamera * cam = nullptr;



    PlayModule * current = nullptr;
    PlayModule * next = nullptr;


    InputController * gameController= nullptr;

    TestAiController * controller2 = nullptr;



    //TODO replace thses with interfaces
    std::vector<std::function<void()> >switchCurrentCallbacks;





private:


    //Player retained copy
    Player * mainPlayer = nullptr;


    void switchCurrentModule();

    void loadCurrentModule();
    void loadNextModule();
    void checkIsCurrentSafelyDone(float delta);


private:
    using cocos2d::Node::setPosition;
    using cocos2d::Node::setAnchorPoint;
    using cocos2d::Node::setContentSize;
    size_t currentLevel;



    //TODO tune the value of this constant here
    //LIMIT this constant based on MAX PLAYER SPEED
    float checkInterval = 0.1f;
};


#endif //MYGAME_MODULECONTAINER_HPP
