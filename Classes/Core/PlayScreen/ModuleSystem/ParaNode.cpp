//
// Created by ashish on 5/24/16.
//

#include "ParaNode.hpp"
#include "PlayModule.hpp"
#include "../MainCamera.hpp"

USING_NS_CC;

ParaNode *ParaNode::create(MainCamera *cam ,const cocos2d::Size & size,const Vec2 & initPos,const Vec2 & endPos,const Dir & dir) {

    auto p = new(std::nothrow)ParaNode();
    if(p && p->init(cam,size,initPos,endPos,dir)){
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return p;

}


bool ParaNode::init(MainCamera *cam ,const cocos2d::Size & size,const Vec2 & initPos,const Vec2 & endPos,const Dir & dir) {
    if(!Node::init()){
        return false;
    }

    setContentSize(size);
    setPosition(initPos);
    this->endPos = endPos;
    this->dir = dir;
    this->cam = cam;



    return true;
}


void ParaNode::onEnter() {
    Node::onEnter();
    CCASSERT(dynamic_cast<PlayModule*>(_parent)!= nullptr,"Parent should be a Play Module");
    parentModule = (PlayModule*)_parent;

    parentModule->addStableListener(this);
    auto mySize = getContentSize();
    auto pSize  = _parent->getContentSize();
    auto sSize  = _director->getWinSize();
    auto pPos = _parent->getPosition();
    //module currently on screen
    if(cam->projectGL(Vec2to3(_parent->convertToWorldSpace(Vec2::ZERO))).x<=_director->getWinSize().width){
        factor.x = (endPos.x-mySize.width-_position.x)/(endPos.x);

    }
    else {
        factor.x = (endPos.x-mySize.width-_position.x)/(sSize.width+endPos.x);


    }


    factor.y = (mySize.height/pSize.height);


#ifdef DEBUGGING_APP
    PlatformerGlobals::printVec2("factor is",factor);
#endif

    camPrevPos = cam->getPosition();
}



void ParaNode::visit(cocos2d::Renderer *renderer,const cocos2d::Mat4 &parentTransform,uint32_t parentFlags) {


    if(parentModule->getIsStable() || cam->projectGL(Vec2to3(_parent->convertToWorldSpace(Vec2::ZERO))).x<=_director->getWinSize().width) {
        moveAhead();
    }
    camPrevPos = cam->getPosition();

    Node::visit(renderer, parentTransform, parentFlags);




}


void ParaNode::onStable() {
    camPrevPos = cam->getPosition();
}

void ParaNode::moveAhead() {
    auto currentCamPos = cam->getPosition();
//    PlatformerGlobals::printVec2("displacement is ",currentCamPos - camPrevPos);

    setPosition(getPositionX() + factor.x * (currentCamPos.x - camPrevPos.x),
                getPositionY() + factor.y * (currentCamPos.y - camPrevPos.y));


}





