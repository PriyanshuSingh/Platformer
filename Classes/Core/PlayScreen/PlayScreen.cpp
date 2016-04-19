//
// Created by ashish on 4/10/16.
//

#include "PlayScreen.hpp"
#include "PlatformerGlobals.hpp"
#include "b2PhysicsSystem.hpp"
#include "../../helper/GLES-Render.h"
#include "ModuleSystem/PlayModule.hpp"
#include "ModuleSystem/ModuleContainer.hpp"
#include "GameUiLayer.hpp"
#include "MainCamera.hpp"

USING_NS_CC;
bool PlayScreen::init() {
    if(!Scene::init()){
        return false;
    }



    {

        this->scheduleUpdate();
    }
    //physics stuff
    {
        //one step while walking = 2.1 feet = 0.64m
        //25 steps = one screen => 16m
        //1024/16 = 64
        float ptmRatio = 64;
        physicsSystem = new B2PhysicsSystem(ptmRatio, b2Vec2(0,-9.8f), false);
        debugDraw = new GLESDebugDraw(ptmRatio);
        debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_centerOfMassBit );
        physicsSystem->getWorld()->SetDebugDraw(debugDraw);
    }





    //create camera
    {
        cam = MainCamera::create();
#ifdef DEBUGGING_APP
        auto projection = Director::getInstance()->getProjection();
        if(projection == Director::Projection::_3D){
            cocos2d::log("in 3d world");
        }
#endif
        this->addChild(cam);
        cam->setCameraFlag(CameraFlag::USER1);
    }




    {
        container = ModuleContainer::create(physicsSystem,cam);
        this->addChild(container);
    }








    //set camera flag in end after adding all the game stuff
    {
        this->setCameraMask((unsigned short)CameraFlag::USER1,true);

    }



    //default camera stuff here
    {
        uiLayer = GameUiLayer::create();
        this->addChild(uiLayer);





    }
















#ifdef DEBUGGING_APP
    {
        sprite = Sprite::create("HelloWorld.png");
        this->addChild(sprite, 10);

        debugDrawerCocos = DrawNode::create();
        this->addChild(debugDrawerCocos);
    }
#endif





















    return true;
}

//game main update loop
void PlayScreen::update(float delta){




    //TODO stuff applied on player here
    //forces and stuff

    //appy forces,Do Box2D stuff here
    container->prePhysicsUpdate(delta);

    physicsSystem->update(delta);


    //update cocos2d-x structures
    container->postPhysicsUpdate(delta);
    //update position of player and stuff here





}

#ifdef DEBUGGING_APP
void PlayScreen::draw(Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
    Node::draw(renderer, transform, flags);


    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when setting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

    director->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW,container->getNodeToWorldTransform());







//    cocos2d::log("transpose print in row order");
//    for(int i=0;i<4;i++){
//        cocos2d::log("%f  %f %f %f",modelViewMV.m[i],modelViewMV.m[4+i],modelViewMV.m[8+i],modelViewMV.m[12+i]);
//    }


    modelViewMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

    customCommand.init(_globalZOrder);
    customCommand.func = CC_CALLBACK_0(PlayScreen::onDraw, this);
    renderer->addCommand(&customCommand);

    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);





    std::vector<Node * >nodes;
    nodes.push_back(sprite);
    PlatformerGlobals::drawNodeDebug(debugDrawerCocos,nodes);
}


void PlayScreen::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags) {





    // quick return if not visible. children won't be drawn.
    if (!_visible)
    {
        return;
    }

    uint32_t flags = processParentFlags(parentTransform, parentFlags);

    // IMPORTANT:
    // To ease the migration to v3.0, we still support the Mat4 stack,
    // but it is deprecated and your code should not rely on it
    _director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    _director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);

    bool visibleByCamera = isVisitableByVisitingCamera();


    if(!_children.empty())
    {
        sortAllChildren();
        // draw children zOrder < 0
        for(int i = 0; i < _children.size(); ++i )
        {
            auto node = _children.at(i);

            if (node )
                node->visit(renderer, _modelViewTransform, flags);

        }




    }


    if (visibleByCamera)
    {
        this->draw(renderer, _modelViewTransform, flags);
    }

    _director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);


}


void PlayScreen::onDraw() {

    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when setting matrix stack");

    auto oldMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, modelViewMV);
    physicsSystem->getWorld()->DrawDebugData();
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);




}

#endif


PlayScreen::~PlayScreen() {

    CC_SAFE_DELETE(physicsSystem);

#ifdef DEBUGGING_APP
    CC_SAFE_DELETE(debugDraw);
#endif


}








