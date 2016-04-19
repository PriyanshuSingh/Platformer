//
// Created by ashish on 4/10/16.
//

#ifndef MYGAME_PLAYSCREEN_HPP
#define MYGAME_PLAYSCREEN_HPP

#include "PlatformerGlobals.hpp"
#include "cocos2d.h"

class GameUiLayer;
class ModuleLayer;
class B2PhysicsSystem;
class Box2DDebugDraw;
class GLESDebugDraw;
class PlayModule;
class ModuleContainer;
class MainCamera;




class PlayScreen:public cocos2d::Scene {
public:
    CREATE_FUNC(PlayScreen);
    bool init()override;





    void update(float delta) override;
    virtual ~PlayScreen();




private:
    //Ownership
    ModuleContainer * container = nullptr;
    B2PhysicsSystem * physicsSystem = nullptr;
    //U.I stuff
    GameUiLayer * uiLayer = nullptr;
    MainCamera * cam = nullptr;





#ifdef DEBUGGING_APP
public:
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    //TODO make sure this method is not the release it Over writes defualt cocos2d-x order of drawing NODES!!
    void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override;

private:
    cocos2d::Mat4 modelViewMV;
    cocos2d::CustomCommand customCommand;
    GLESDebugDraw * debugDraw;
    void onDraw();
    cocos2d::DrawNode * debugDrawerCocos = nullptr;
    cocos2d::Sprite *sprite = nullptr;
#endif


};


#endif //MYGAME_PLAYSCREEN_HPP
