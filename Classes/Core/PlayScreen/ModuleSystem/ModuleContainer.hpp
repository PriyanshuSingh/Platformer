//
// Created by ashish on 4/11/16.
//

#ifndef MYGAME_MODULECONTAINER_HPP
#define MYGAME_MODULECONTAINER_HPP

#include "cocos2d.h"
#include "../PlatformerGlobals.hpp"
#include "../Physics/PhysicsUpdatable.hpp"
#include "PlayModule.hpp"

class B2PhysicsSystem;
class Player;
class MainCamera;
class InputController;
class TestAiController;

class ModuleContainer :public cocos2d::Node, public PhysicsUpdatable {
public:
    ADD_CHILD_MASK(cocos2d::Node)

    virtual void onEnter() override;;
    static ModuleContainer * create(MainCamera * cam);
    bool init(MainCamera * cam);

    ~ModuleContainer();

    CC_DEPRECATED_ATTRIBUTE void move(float xDisplacement,float zoomFactor);


//public API
    void onEnterLastModule();
    //TODO remove this and use a interface instead like for Player controllers
    void addSwitchCurrentCallback(const  std::function<void()> & callback);
    PlayModule *getCurrentModule();




    void prePhysicsUpdate(float delta)override;
    void postPhysicsUpdate(float delta)override;


private:
    enum DRAWORDER{
        BACKGROUND = 0,
        MIDDLEGROUND = 10,
        FOREGROUND = 20
    };



    //weak references
    MainCamera * cam = nullptr;



    PlayModule * current = nullptr;
    PlayModule * next = nullptr;


    InputController * gameController= nullptr;

    TestAiController * controller2 = nullptr;



    //TODO replace these with interfaces
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
