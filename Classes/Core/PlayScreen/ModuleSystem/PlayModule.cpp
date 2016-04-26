//
// Created by ashish on 4/11/16.
//


#include "../PlatformerGlobals.hpp"
#include "PlayModule.hpp"
#include "../Physics/b2PhysicsSystem.hpp"
#include "../../../rubeStuff/b2dJson.h"
#include "../Player/Player.hpp"
#include "ModuleContainer.hpp"
#include "../MainCamera.hpp"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;




bool PlayModule::init(const staticInfo & info,B2PhysicsSystem * system,MainCamera * cam,const b2Vec2 & offset) {
    if(!Layer::init()){
        return false;
    }
    {
        ignoreAnchorPointForPosition(false);
        setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        adjustOffset = Director::getInstance()->getWinSize().width*0.1f;
        this->cam = cam;
        this->system = system;
        this->boxInitOffset = offset;
    }

    auto json = system->addJsonObject(info.rubeInfo);
    b2Body * boundingBody = json.getBodyByName("BoundingBox");
    CCASSERT(boundingBody,"Bounding Body does not exist");

    {
        //update bounding box size based on the Box2D body
        setContentSize(Size(system->box2DToScreen(boundingBody->GetPosition())));
        json.getAllBodies(bodies);
        for(auto & b: bodies){
            //TODO decide nullptr or this
            b->SetUserData(nullptr);
        }
#ifdef DEBUGGING_APP
        std::vector<b2Joint*>joints;
        json.getAllJoints(joints);
        CCASSERT(joints.size()<=0,"Trying to add joint to static Data to Play Module");
#endif



        //set ther user data null and adjust offset
        initBodies(offset);

    }
    //load corresponding sprites and stuff from CS Loader
    {


        auto rootNode = CSLoader::createNode(info.cocosInfo);
        addChild(rootNode,DRAWORDER::MIDDLEGROUND);

    }


    //non-physics stuff
    {
        foreGroundObject = ParallaxNode::create();
        backGroundObject = ParallaxNode::create();
        addChild(foreGroundObject, DRAWORDER::FOREGROUND);
        addChild(backGroundObject, DRAWORDER::BACKGROUND);
    }










    return true;
}
void PlayModule::onCoordsStable() {
    //zero out offset
    boxInitOffset = b2Vec2();
    boxInitOffset.SetZero();
}

PlayModule::~PlayModule() {


    //TODO delete only bodies because these are pure statics

    if(B2PhysicsSystem::isSystemActive()) {

        for (auto &b:bodies) {
            system->getWorld()->DestroyBody(b);

        }
    }
    cocos2d::log("called play module destructor callback");




}





void PlayModule::initBodies(const b2Vec2 &offset) {


    for (size_t i = 0; i< bodies.size(); ++i)
    {
        system->addOffset(bodies.at(i),offset);
        bodies.at(i)->SetUserData(nullptr);
    }



}


std::vector<b2Body *> & PlayModule::getAddedBodies() {
    return bodies;
}


float PlayModule::getPlayerPositionPercentage() {

    if(player!= nullptr){
        //TODO note it doesn't take player anchor point into consideration
        //(to do that add (1-anchorpint.x)*player->getContentSize
        return player->getPosition().x/getContentSize().width;


    }
    return -1;
}


void PlayModule::postPhysicsUpdate(float delta) {



    cam->move2D(delta,Vec2(player->getDeltaMovement().x,0));




}



float PlayModule::getOffset() {
    return adjustOffset;
}

void PlayModule::setCoordinatesStabilized(bool stable) {
    if(stable == coordsStable)
        return;
    coordsStable = stable;
    if(coordsStable)onCoordsStable();

}



void PlayModule::addPlayer(Player *player) {

    addChild(player,DRAWORDER::PLAYER);
    this->player = player;



}


void PlayModule::removePlayer() {
    CCASSERT(player,"Removing player when there is none");
    removeChild(player);
    player = nullptr;


}



bool PlayModule::getIsStable() const{
    return coordsStable;

}












