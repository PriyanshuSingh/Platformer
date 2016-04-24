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
//    // Find out the absolute path for the file
//    std::string path = FileUtils::getInstance()->fullPathForFilename(info.rubeInfo.c_str());
//
//
//    // This will print out the actual location on disk that the file is read from.
//    // When using the simulator, exporting your RUBE scene to this folder means
//    // you can edit the scene and reload it without needing to restart the app.
//#ifdef DEBUGGING_APP
//    CCLOG("Full path is: %s", path.c_str());
//#endif
//    // Create the world from the contents of the RUBE .json file. If something
//    // goes wrong, m_world will remain NULL and errMsg will contain some info
//    // about what happened.
//    b2dJson json;
//    std::string errMsg;
//    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(path);
//    json.readFromString(jsonContent, errMsg,system->getWorld());
//
//
//#ifdef DEBUGGING_APP
//
//    if(errMsg.empty()){
//        cocos2d::log("no error occured json loaded ok");
//    }
//    else{
//        cocos2d::log("Error occured : %s",errMsg.c_str());
//    }
//#endif

    auto json = system->addJsonObject(info.rubeInfo);
    b2Body * boundingBody = json.getBodyByName("BoundingBox");
    CCASSERT(boundingBody,"Bounding Body does not exist");
    {
        //update bounding box size based on the Box2D body
        setContentSize(Size(system->box2DToScreen(boundingBody->GetPosition())));


        json.getAllBodies(bodies);
        json.getAllJoints(joints);




        //add bodies here only for debugging when they don't need any visual representation
        //shift bodies ahead(In Rube they were made relative to world Origin).
        addOffsetBodiesAndJoints(offset);

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


    //TODO delete joints before bodies

//    if(B2PhysicsSystem::isSystemActive()) {
//        for (auto &j:joints) {
//            system->getWorld()->DestroyJoint(j);
//        }
//
//        for (auto &b:bodies) {
//            system->getWorld()->DestroyBody(b);
//
//        }
//    }
    cocos2d::log("called play module destructor callback");




}





void PlayModule::addOffsetBodiesAndJoints(const b2Vec2 &offset) {


    for (size_t i = 0; i< bodies.size(); ++i)
    {
        system->addOffset(bodies.at(i),offset);

    }
    for (size_t i= 0; i < joints.size(); ++i)
    {

        system->addOffset(joints.at(i),offset);

    }



}


std::vector<b2Body *> & PlayModule::getAddedBodies() {
    return bodies;
}

std::vector<b2Joint *> & PlayModule::getAddedJoints() {
    return joints;
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
    player->syncPositionWithPhysics();



}


void PlayModule::removePlayer() {
    CCASSERT(player,"Removing player when there is none");
    removeChild(player);
    player = nullptr;


}



bool PlayModule::getIsStable() const{
    return coordsStable;

}












