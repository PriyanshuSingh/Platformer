//
// Created by ashish on 4/11/16.
//


#include "../PlatformerGlobals.hpp"
#include "Player.hpp"
#include "../b2PhysicsSystem.hpp"
#include "../ModuleSystem/ModuleContainer.hpp"
#include "../ModuleSystem/PlayModule.hpp"
#include "PlayerController.hpp"
#include "PlayerCommand.hpp"

USING_NS_CC;
Player *Player::create(ModuleContainer * container,B2PhysicsSystem *system) {



    auto player = new (std::nothrow) Player();
    if (player && player->init(container,system))
    {
        player->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(player);
    }
    return player;

}

bool Player::init(ModuleContainer * container,B2PhysicsSystem *system) {
    if(!Node::init()){
        return false;
    }

    {

        this->system = system;
        this->container = container;
        this->dead = false;
        setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        setName(PlatformerGlobals::MainPlayerName);

    }

    std::string fileName("platformer/player/player.json");

    // Find out the absolute path for the file
    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(fileName.c_str());


    // This will print out the actual location on disk that the file is read from.
    // When using the simulator, exporting your RUBE scene to this folder means
    // you can edit the scene and reload it without needing to restart the app.
#ifdef DEBUGGING_APP
    CCLOG("Full path is: %s", fullpath.c_str());
#endif
    //If something
    // goes wrong, m_world will remain NULL and errMsg will contain some info
    // about what happened.
    b2dJson json;
    std::string errMsg;
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(fullpath);
    json.readFromString(jsonContent, errMsg,system->getWorld());


#ifdef DEBUGGING_APP

    if(errMsg.empty()){
        cocos2d::log("no error occured json loaded ok");
    }
    else{
        cocos2d::log("Error occured : %s",errMsg.c_str());
    }
#endif

    //main body
    {

        playerInfo.rubeInfo = jsonContent;
        mainBody = json.getBodyByName("MainBody");

        CCASSERT(mainBody, "Bounding Body does not exist");
        mainBody->SetUserData(this);







        saveState();

        //TODO very imp for you to reset the user data back to player
        playerInfo.mainBody = json.b2j(mainBody);
        //save joint info and stuff here

    }
    //fixtures
    {

        groundFixture = json.getFixtureByName("FootFixture");

    }

    mainBody->SetTransform(b2Vec2(10,10),0);



    prevPosition = mainBody->GetPosition();
















    return true;
}

void Player::prePhysicsUpdate(float delta) {


    if(!dead){



    }
    //execute commands
    for(auto & p:controllers){
        auto com = p->getCommand();
        if(com!= nullptr)
            com->execute();
    }
}


void Player::postPhysicsUpdate(float delta) {



    if(dead){

        playerDeltaMovement = Vec2(0,0);
        return;
    }


    //TODO discuss to put this in pre or post



    //TODO this approach wont work with slow motion effects
    //The Displacement approach gets fucked up when the world origin
    //shifts.What To DO?Decision Pending..
    playerDeltaMovement = system->box2DToScreen(delta*mainBody->GetLinearVelocity());
    prevPosition=mainBody->GetPosition();


    //setup player state vars

    grounded = false;
    for (b2ContactEdge* edge = mainBody->GetContactList(); edge; edge = edge->next){
        auto fixA = edge->contact->GetFixtureA();
        auto fixB = edge->contact->GetFixtureB();

        if(fixA == groundFixture || fixB == groundFixture){
            if(edge->contact->IsTouching()){
                grounded = true;
                break;
            }
        }

    }


}





Player::~Player() {


    //TODO see if destroy stuff here
}

void Player::setParent(cocos2d::Node *parent) {
    Node::setParent(parent);

    //this is because when child is removed from Play module
    //remove child calls the setParent(nullptr);
    if(parent == nullptr){
        parentModule = nullptr;
        return;
    }
    CCASSERT(dynamic_cast<PlayModule*>(parent)!= nullptr,"Player being added not to a Play Module!!");
    parentModule = dynamic_cast<PlayModule*>(parent);



}

void Player::syncPositionWithPhysics() {

    //just set the sprites position to physics bodies position

}


cocos2d::Vec2 Player::getDeltaMovement() {


    return playerDeltaMovement;
}

void Player::addController(PlayerController *controller) {
    auto it = std::find(controllers.begin(), controllers.end(), controller);
    if(it == controllers.end()){
        controllers.push_back(controller);
        return;
    }
    CCASSERT(false,"ADDING SAME CONTROLLER AGAIN!");

}


void Player::removeController(PlayerController *controller) {
    auto it = std::find(controllers.begin(), controllers.end(), controller);

    if (it != controllers.end()) {
        using std::swap;

        // swap the one to be removed with the last element
        swap(*it, controllers.back());
        controllers.pop_back();
    }
}

void Player::saveState() {

}



void Player::bringToLife(const b2Vec2 &pos,float degrees) {



    b2dJson json;
    std::string errorInfo;
    json.readFromString(playerInfo.rubeInfo,errorInfo,system->getWorld());

    if(errorInfo.empty()){
        cocos2d::log("no error occured AGAIN json loaded ok");
    }
    else{
        cocos2d::log("Error occured AGAIN : %s",errorInfo.c_str());
    }
    mainBody = json.getBodyByName("MainBody");
    mainBody->SetUserData(this);
    groundFixture = json.getFixtureByName("FootFixture");

    mainBody->SetTransform(pos,-1*CC_DEGREES_TO_RADIANS(degrees));

    this->dead = false;

}

void Player::killMe() {


    cocos2d::log("position is %f %f",system->box2DToScreen(mainBody->GetPosition().x),system->box2DToScreen(mainBody->GetPosition().y));
    //TODO destroy joints before here
    system->getWorld()->DestroyBody(mainBody);
    //nullify pointers
    mainBody = nullptr;
    groundFixture = nullptr;
    this->dead = true;




}