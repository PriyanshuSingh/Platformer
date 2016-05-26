//
// Created by ashish on 5/24/16.
//

#include "ParaNode.hpp"
#include "PlayModule.hpp"
#include "../MainCamera.hpp"

USING_NS_CC;

ParaNode *ParaNode::create(MainCamera *cam ,const cocos2d::Size & size,const Dir & dir) {

    auto p = new(std::nothrow)ParaNode();
    if(p && p->init(cam,size,dir)){
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return p;

}


bool ParaNode::init(MainCamera *cam ,const cocos2d::Size & size,const Dir & dir) {
    if(!Node::init()){
        return false;
    }
    setContentSize(size);
    this->dir = dir;
    this->cam = cam;






    scheduleUpdate();
    return true;
}


void ParaNode::onEnter() {
    Node::onEnter();
    CCASSERT(dynamic_cast<PlayModule*>(_parent)!= nullptr,"Parent should be Play Module");
    parentModule = (PlayModule*)_parent;
    parentModule->addStableListener(this);


    auto mySize = getContentSize();
    auto pSize = _parent->getContentSize();
    factor = Vec2(1-mySize.width/pSize.width,1-mySize.height/pSize.height);
    PlatformerGlobals::printVec2("factor is",factor);

    camPrevPos = cam->getPosition();


}


void ParaNode::update(float delta) {

    auto hold = cam->projectGL(Vec2to3(convertToWorldSpace(Vec2::ZERO)));

    if (hold.x <= _director->getWinSize().width) {

    }


}
void ParaNode::visit(cocos2d::Renderer *renderer,const cocos2d::Mat4 &parentTransform,uint32_t parentFlags) {


    Node::visit(renderer, parentTransform, parentFlags);

    auto hold = cam->projectGL(Vec2to3(convertToWorldSpace(Vec2::ZERO)));

    if(hold.x<=_director->getWinSize().width){
//        cocos2d::log("this happened");


        auto currentCamPos = cam->getPosition();
        setPosition(getPositionX() + factor.x * (currentCamPos.x - camPrevPos.x),
                    getPositionY() + factor.y * (currentCamPos.y - camPrevPos.y));
        camPrevPos = currentCamPos;


    }
    else{
        camPrevPos = cam->getPosition();
    }


}


void ParaNode::onStable() {
    camPrevPos = cam->getPosition();
}

