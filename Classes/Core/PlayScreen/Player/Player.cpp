//
// Created by ashish on 4/11/16.
//


#include "../PlatformerGlobals.hpp"
#include "Player.hpp"
#include "../Physics/b2PhysicsSystem.hpp"
#include "../ModuleSystem/ModuleContainer.hpp"
#include "../ModuleSystem/PlayModule.hpp"
#include "PlayerController.hpp"
#include "PlayerCommand.hpp"

USING_NS_CC;
Player *Player::create(ModuleContainer * container,B2PhysicsSystem *system,const b2Vec2 & initPosition) {



    auto player = new (std::nothrow) Player();
    if (player && player->init(container,system,initPosition))
    {
        player->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(player);
    }
    return player;

}

bool Player::init(ModuleContainer * container,B2PhysicsSystem *system,const b2Vec2 & initPosition) {
    auto json = system->addJsonObject("Platformer/Player/player.json");


    if(!PhysicsActor::init(system,ActorType::Player,initPosition,json)){
        return false;
    }
    setTag(PhysicsActor::MainPlayerTag);

    //properties
    {
        this->container = container;

        setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    }

    //main body
    {

        mainBody = json.getBodyByName("MainBody");
        CCASSERT(mainBody, "Bounding Body does not exist");

    }

    //fixtures
    {

        groundFixture = json.getFixtureByName("FootFixture");

    }
//    setupPlayerSprite(&json);

    return true;
}

void Player::prePhysicsUpdate(float delta) {


    //execute commands
    for(auto & p:controllers){
        auto com = p->getCommand();
        if(com!= nullptr)
            com->execute();
    }
}


void Player::postPhysicsUpdate(float delta) {



    if(dead){

        playerDeltaMovement = Vec2::ZERO;
        return;
    }





    //TODO this approach wont work with slow motion effects
    //The Displacement approach gets fucked up when the world origin
    //shifts.What To DO?Decision Pending..=>Use cocos2d-x getWorldPosition for displacement

//    auto currentPos = _parent->convertToWorldSpace(getPosition());
//    playerDeltaMovement = currentPos-prevPosition;
//    prevPosition = currentPos;

    playerDeltaMovement = system->box2DToScreen(delta*mainBody->GetLinearVelocity());

    //setup player state vars

    grounded = false;
    for (b2ContactEdge* edge = mainBody->GetContactList(); edge; edge = edge->next){
        auto fixA = edge->contact->GetFixtureA();
        auto fixB = edge->contact->GetFixtureB();

        if(fixA == groundFixture && !fixB->IsSensor()){
            if(edge->contact->IsTouching()){
                grounded = true;
                break;
            }
        }

        if(fixB == groundFixture && !fixA->IsSensor()){
            if(edge->contact->IsTouching()){
                grounded = true;
                break;
            }
        }

    }

    setPosition(box2DToActorParentSpace(mainBody->GetPosition()));
}


void Player::onEnter() {
    PhysicsActor::onEnter();

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



void Player::bringToLife(const b2Vec2 &pos) {



    auto json = system->addJsonObject("Platformer/Player/player.json");

    //TODO replace this stuff with reinit
    fillBodyAndJoints(json);
    setupPhysicsObjects(pos,true);



    mainBody = json.getBodyByName("MainBody");
    groundFixture = json.getFixtureByName("FootFixture");
    mainBody->SetTransform(pos,0);

    dead = false;

}

void Player::killMe() {


    deleteAllBodiesAndJoints();



    playerDeltaMovement= Vec2::ZERO;
    grounded = attached = false;
    prevPosition = Vec2::ZERO;

    dead = true;

    //nullify pointers
    mainBody = nullptr;
    groundFixture = nullptr;




}
//TODO @Priyanshu Player can have multiple layer of images
void Player::setupPlayerSprite(b2dJson *json) {
    // fill a vector with all images in the RUBE scene
    std::vector<b2dJsonImage*> b2dImages;
    json->getAllImages(b2dImages);
    // loop through the vector, create Sprites for each image and store them in m_imageInfos
    b2dJsonImage *img = b2dImages[0];

    CCLOG("Loading image: %s", img->file.c_str());

    // try to load the sprite image, and ignore if it fails
    playerSprite = Sprite::create(img->file.c_str());
    if (!playerSprite)
        return;

    // add the sprite to Static Sprite and set the render order
    this->addChild(playerSprite);
    //TODO change this.. very bad hack to get PlayModule::DRAWORDER::PLAYER :(
    this->reorderChild(playerSprite, 15);

    // these will not change during simulation so we can set them now
    playerSprite->setFlippedX(img->flip);
    playerSprite->setColor(Color3B(img->colorTint[0], img->colorTint[1], img->colorTint[2]));
    playerSprite->setOpacity(img->colorTint[3]);
    playerSprite->setScale(img->scale*system->getPtmRatio() / playerSprite->getContentSize().height);
    CCLOG(" scale %f\ncontentSize %f %f",playerSprite->getScale(),playerSprite->getContentSize().height,playerSprite->getContentSize().width);
    // create an info structure to hold the info for this image (body and position etc)


    Point pos = Vec2(img->center.x,img->center.y);
    float angle = -img->angle;

    //need to rotate image local center by body angle
    b2Vec2 localPos( pos.x, pos.y );
    b2Rot rot( mainBody->GetAngle() );
    localPos = b2Mul(rot, localPos) + mainBody->GetPosition();
    pos.x = localPos.x;
    pos.y = localPos.y;
    angle += -mainBody->GetAngle();
    playerSprite->setRotation( CC_RADIANS_TO_DEGREES(angle) );
    Vec2 a = system->box2DToScreen(b2Vec2(pos.x,pos.y));
    setPosition(a);
}
