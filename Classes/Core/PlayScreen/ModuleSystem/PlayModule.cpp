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

USING_NS_CC;




bool PlayModule::init(const ModuleInfo & info,B2PhysicsSystem * system,MainCamera * cam,const b2Vec2 & offset) {
    if(!Layer::init()){
        return false;
    }
    {
        ignoreAnchorPointForPosition(false);
        setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        adjustOffset = Director::getInstance()->getWinSize().width*0.1f;
        this->cam = cam;
        this->system = system;

        boxInitOffset = offset;
        box2dToModuleMatrix.translate(Vec2to3(-1*system->box2DToScreen(boxInitOffset)));
        float32 ratio = system->getPtmRatio();
        box2dToModuleMatrix.scale(Vec3(ratio,ratio,0));

    }
    // Find out the absolute path for the file
    std::string path = FileUtils::getInstance()->fullPathForFilename(info.rubeInfo.c_str());


    // This will print out the actual location on disk that the file is read from.
    // When using the simulator, exporting your RUBE scene to this folder means
    // you can edit the scene and reload it without needing to restart the app.
#ifdef DEBUGGING_APP
    CCLOG("Full path is: %s", path.c_str());
#endif
    // Create the world from the contents of the RUBE .json file. If something
    // goes wrong, m_world will remain NULL and errMsg will contain some info
    // about what happened.
    b2dJson json;
    std::string errMsg;
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(path);
    json.readFromString(jsonContent, errMsg,system->getWorld());


#ifdef DEBUGGING_APP

    if(errMsg.empty()){
        cocos2d::log("no error occured json loaded ok");
    }
    else{
        cocos2d::log("Error occured : %s",errMsg.c_str());
    }
#endif

    b2Body * boundingBody = json.getBodyByName("BoundingBox");
    CCASSERT(boundingBody,"Bounding Body does not exist");

//update bounding box size based on the Box2D body
    setContentSize(Size(system->box2DToScreen(boundingBody->GetPosition())));


    json.getAllBodies(bodies);
    json.getAllJoints(joints);




    //TODO remove any body from directly being added to a module(use PhysicsActors)

    //add bodies here only for debugging when they don't need any visual representation
    //shift bodies ahead(In Rube they were made relative to world Origin).
    addOffsetBodiesAndJoints(offset);


    //non-physics stuff
    {
        foreGroundObject = ParallaxNode::create();
        backGroundObject = ParallaxNode::create();
        this->addChild(foreGroundObject, DRAWORDER::FOREGROUND);
        this->addChild(backGroundObject, DRAWORDER::BACKGROUND);
    }










    return true;
}
void PlayModule::onCoordsStable() {


    Mat4 temp;
    temp.translate(Vec2to3(system->box2DToScreen(boxInitOffset)));
    box2dToModuleMatrix = temp*box2dToModuleMatrix;


    //zero out offset
    boxInitOffset = b2Vec2();
    boxInitOffset.SetZero();
    //test body
//    {
//        b2BodyDef testBodyDef;
//        testBodyDef.type = b2_dynamicBody;
//        b2CircleShape c;
//        c.m_radius = 2;
//        auto hold = system->getWorld()->CreateBody(&testBodyDef);
//        hold->SetTransform(b2Vec2(14,9),0);
//        hold->CreateFixture(&c,1.0f);
//        bodies.push_back(hold);
//
//    }


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
        b2Vec2 pos = bodies.at(i)->GetPosition();
        float angle = bodies.at(i)->GetAngle();
        bodies.at(i)->SetTransform(pos+offset,angle);

    }
    //TODO confirm this shift here

    for (size_t i= 0; i < joints.size(); ++i)
    {

        //note the minus here
        joints.at(i)->ShiftOrigin(-offset);
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


void PlayModule::setPosition(const cocos2d::Vec2 &position) {
    Node::setPosition(position);
    //TODO update matrix here
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


cocos2d::Vec2 PlayModule::tranformBox2DToModule(const b2Vec2 &pos) {
    Vec2 hold = BtoC(pos);
    Vec4 hold4(hold.x,hold.y,0,1);
    hold4 = box2dToModuleMatrix*hold4;
    return Vec2(hold4.x,hold4.y);
}

bool PlayModule::getIsStable() const{
    return coordsStable;

}












