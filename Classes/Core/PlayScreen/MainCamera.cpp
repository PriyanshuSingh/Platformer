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
    if(!Camera::initDefault())
        return false;




    defaultPos = getPosition3D();
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



void MainCamera::move2D(float delta,const cocos2d::Vec2 &displacement) {



    //TODO use EASING STUFF to make it great :)
    auto action = MoveBy::create(delta,displacement);
    this->runAction(action);




}





void MainCamera::zoomIn(float delta, float depth, const cocos2d::Vec2 &focusPoint) {

}


void MainCamera::zoomOut(float delta, float depth, const cocos2d::Vec2 &returnPoint) {

}

Vec2 MainCamera::getStart2dPosition() {
    return Vec2(defaultPos.x,defaultPos.y);
}




