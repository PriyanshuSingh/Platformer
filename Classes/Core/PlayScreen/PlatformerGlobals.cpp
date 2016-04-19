//
// Created by ashish on 4/16/16.
//




#include "PlatformerGlobals.hpp"
#include "cocos2d.h"
USING_NS_CC;
const std::string PlatformerGlobals::MainPlayerName = "MainPlayer";



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

