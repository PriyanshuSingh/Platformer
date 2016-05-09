//
// Created by ashish on 4/13/16.
//

#include "MainCamera.hpp"


USING_NS_CC;


MainCamera *MainCamera::create() {
    auto camera = new (std::nothrow) MainCamera();
    camera->init();
    camera->autorelease();
    //IMPORTANT TO SET DEPTH to allow GUI to be rendered at the top
    camera->setDepth(-1.f);
    return camera;
}

MainCamera::~MainCamera() {




}

bool MainCamera::init() {
    auto size = Director::getInstance()->getWinSize();
    float zeye = Director::getInstance()->getZEye();

    if(!initPerspective(60, (GLfloat)size.width / size.height, 10, zeye + size.height / 2.0f)){
        return false;
    }

    Vec3 eye(size.width/2, size.height/2.0f, zeye), center(size.width/2, size.height/2, 0.0f), up(0.0f, 1.0f, 0.0f);
    setPosition3D(eye);
    lookAt(center, up);


    defaultPos = getPosition3D();

    backGround = Sprite::create("background.jpg");
    backGround->setOpacity(64);
    this->addChild(backGround);
    auto winSize = Director::getInstance()->getWinSize();
    backGround->setAnchorPoint(Vec2(0,0));
    backGround->setPosition3D(Vec3(-winSize.width/2,-winSize.height/2,-getPositionZ()));

    return true;
}


void MainCamera::setBack2DPosition() {

    setPosition3D(Vec3(defaultPos.x,defaultPos.y,getPositionZ()));
}

void MainCamera::setBack2DPosition(float delta) {

}


void MainCamera::setBackToDefault(float delta) {
    return;
}




void MainCamera::setZoomBackToDefault(float detla) {
    return;
}


cocos2d::Vec2 MainCamera::getDownVector() {
    Vec2 ans(-1*Vec2::UNIT_Y);
    ans.rotate(Vec2::ZERO,-CC_DEGREES_TO_RADIANS(getRotation()));
    return ans;
}


void MainCamera::move2D(float delta,const cocos2d::Vec2 &displacement) {



    //TODO use EASING STUFF to make it great :)
    auto action = MoveBy::create(delta,displacement);
    this->runAction(action);

    auto action2 = MoveBy::create(delta,-1*displacement*(1-0.4));
    backGround->runAction(action2);




}





void MainCamera::zoomIn(float delta, float depth, const cocos2d::Vec2 &focusPoint) {

}


void MainCamera::zoomOut(float delta, float depth, const cocos2d::Vec2 &returnPoint) {

}

Vec2 MainCamera::getStart2dPosition() {
    return Vec2(defaultPos.x,defaultPos.y);
}




