//
// Created by ashish on 4/16/16.
//




#include "PlatformerGlobals.hpp"
#include "cocos2d.h"

USING_NS_CC;

const std::string PlatformerGlobals::MainPlayerName = "MainPlayer";









b2Vec2 CtoB(const cocos2d::Vec2 &cocosVec) {
    return b2Vec2(cocosVec.x,cocosVec.y);
}

cocos2d::Vec2 BtoC(const b2Vec2 &bVec) {
    return cocos2d::Vec2(bVec.x,bVec.y);
}

cocos2d::Vec3 Vec2to3(const cocos2d::Vec2 &cocosVec) {
    return cocos2d::Vec3(cocosVec.x,cocosVec.y,0);

}

float AngleBToC(float angle){
    return -CC_RADIANS_TO_DEGREES(angle);
}


void PlatformerGlobals::drawNodeDebug(cocos2d::DrawNode * debugDrawer,const std::vector<cocos2d::Node *> & nodes) {


    debugDrawer->clear();
    for(const auto & node:nodes) {


        Size sizeofNode = node->getContentSize();
        Mat4 affineMat = node->getNodeToWorldTransform();

        Vec4 leftBottom = affineMat * (Vec4(0, 0, 0, 1));
        Vec4 leftTop = affineMat * (Vec4(0, Vec2(sizeofNode).y, 0, 1));
        Vec4 rightTop = affineMat * (Vec4(Vec2(sizeofNode).x, Vec2(sizeofNode).y, 0, 1));
        Vec4 rightBottom = affineMat * (Vec4(Vec2(sizeofNode).x, 0, 0, 1));


        debugDrawer->drawLine(Vec2(leftBottom.x, leftBottom.y), Vec2(leftTop.x, leftTop.y), Color4F::RED);
        debugDrawer->drawLine(Vec2(leftTop.x, leftTop.y), Vec2(rightTop.x, rightTop.y), Color4F::GREEN);
        debugDrawer->drawLine(Vec2(rightTop.x, rightTop.y), Vec2(rightBottom.x, rightBottom.y), Color4F::BLUE);
        debugDrawer->drawLine(Vec2(rightBottom.x, rightBottom.y), Vec2(leftBottom.x, leftBottom.y), Color4F::ORANGE);


    }
}


void PlatformerGlobals::printVec2(const std::string &prefix, const cocos2d::Vec2 &vec) {
    cocos2d::log("%s %f %f",prefix.c_str(),vec.x,vec.y);
}


void PlatformerGlobals::printMat4RowOrder(const std::string &prefix, const cocos2d::Mat4 & mat) {

    cocos2d::log("%s",prefix.c_str());

    for(int i=0;i<4;i++){
        cocos2d::log("%f %f %f %f",mat.m[i+0*4],mat.m[i+1*4],mat.m[i+2*4],mat.m[i+3*4]);
    }
}


