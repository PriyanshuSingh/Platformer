//
// Created by ashish on 4/11/16.
//


#include "../PlatformerGlobals.hpp"
#include "PlayModule.hpp"
#include "../Physics/b2PhysicsSystem.hpp"
#include "../../../rubeStuff/b2dJson.h"
#include "../../../rubeStuff/b2dJsonImage.h"
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

        //Static Sprite
        staticSprites = Sprite::create();
        staticSprites->setPosition(Vec2::ZERO);
        addChild(staticSprites,DRAWORDER::MIDDLEGROUND);
        //load the images and set
        loadImagesFromRube(&json);

    }
    //load corresponding sprites and stuff from CS Loader
    //NOTE: Avoid same images in cocosStudio and RUBE
    {

        if(info.cocosInfo != "") {
            auto rootNode = CSLoader::createNode(info.cocosInfo);
            addChild(rootNode, DRAWORDER::MIDDLEGROUND);
        }

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
    cocos2d::log("on coords stable Play module");
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

    //Deleting info
    for (auto info: m_imageInfos){
        delete info;
    }

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


void PlayModule::loadImagesFromRube(b2dJson *json) {
    // fill a vector with all images in the RUBE scene
    std::vector<b2dJsonImage*> b2dImages;
    json->getAllImages(b2dImages);
    // loop through the vector, create Sprites for each image and store them in m_imageInfos
    for (int i = 0; i < b2dImages.size(); i++) {
        b2dJsonImage *img = b2dImages[i];

        CCLOG("Loading image: %s", img->file.c_str());

        // try to load the sprite image, and ignore if it fails
        Sprite *sprite = Sprite::create(img->file.c_str());
        if (!sprite)
            continue;

        // add the sprite to Static Sprite and set the render order
        staticSprites->addChild(sprite);
        staticSprites->reorderChild(sprite, img->renderOrder  + 1); //watch out - RUBE render order is float but cocos2d uses integer (why not float?)

        // these will not change during simulation so we can set them now
        sprite->setFlippedX(img->flip);
        sprite->setColor(Color3B(img->colorTint[0], img->colorTint[1], img->colorTint[2]));
        sprite->setOpacity(img->colorTint[3]);
        sprite->setScale(img->scale*system->getPtmRatio() / sprite->getContentSize().height);
        CCLOG(" scale %f\ncontentSize %f %f",sprite->getScale(),sprite->getContentSize().height,sprite->getContentSize().width);
        // create an info structure to hold the info for this image (body and position etc)
        RUBEImageInfo *imgInfo = new RUBEImageInfo;
        imgInfo->sprite = sprite;
        imgInfo->name = img->name;
        imgInfo->body = img->body;
        imgInfo->scale = img->scale;
        imgInfo->aspectScale = img->aspectScale;
        imgInfo->angle = img->angle;
        imgInfo->center = Vec2(img->center.x, img->center.y);
        imgInfo->opacity = img->opacity;
        imgInfo->flip = img->flip;
        for (int n = 0; n < 4; n++)
            imgInfo->colorTint[n] = img->colorTint[n];

        // add the info for this image to the list
        m_imageInfos.insert(imgInfo);
    }
    setImagePositionsFromPhysicsBodies();
}

void PlayModule::setImagePositionsFromPhysicsBodies() {
    for (std::set<RUBEImageInfo*>::iterator it = m_imageInfos.begin(); it != m_imageInfos.end(); ++it) {
        RUBEImageInfo* imgInfo = *it;
        Point pos = imgInfo->center;
        float angle = -imgInfo->angle;
        if ( imgInfo->body ) {
            //need to rotate image local center by body angle
            b2Vec2 localPos( pos.x, pos.y );
            b2Rot rot( imgInfo->body->GetAngle() );
            localPos = b2Mul(rot, localPos) + imgInfo->body->GetPosition();
            pos.x = localPos.x;
            pos.y = localPos.y;
            angle += -imgInfo->body->GetAngle();
        }
        imgInfo->sprite->setRotation( CC_RADIANS_TO_DEGREES(angle) );
        Vec2 a = system->box2DToScreen(b2Vec2(pos.x,pos.y));
        imgInfo->sprite->setPosition(a);
        CCLOG("pos for sprite is %f %f",a.x, a.y );
    }
}
