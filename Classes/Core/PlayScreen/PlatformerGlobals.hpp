//
// Created by ashish on 4/12/16.
//

#ifndef MYGAME_PLATFORMERGLOBALS_HPP
#define MYGAME_PLATFORMERGLOBALS_HPP

#include <string>
#include <vector>
#include "Box2D/Common/b2Math.h"


//#define DEBUGGING_APP

//#define FIXED_TIMESTEP


#define ADD_CHILD_MASK(__PARENT__) \
virtual void addChild(cocos2d::Node *child) override {\
    __PARENT__::addChild(child);                   \
}                                                     \
virtual void addChild(cocos2d::Node *child, int localZOrder) override {\
    __PARENT__::addChild(child, localZOrder);                          \
}                                                                      \
virtual void addChild(cocos2d::Node *child, int localZOrder, int tag) override {\
    __PARENT__::addChild(child, localZOrder, tag);                              \
    child->setCameraMask(getCameraMask());                                      \
}                                                                               \
virtual void addChild(cocos2d::Node *child, int localZOrder, const std::string &name) override {\
    __PARENT__::addChild(child, localZOrder, name);                                             \
    child->setCameraMask(getCameraMask());                                                      \
}





namespace cocos2d {
    class Node;
    class DrawNode;
    class Vec2;
    class Vec3;
    class Mat4;
}

b2Vec2 CtoB(const cocos2d::Vec2 & cocosVec);
cocos2d::Vec2 BtoC(const  b2Vec2 & bVec);
cocos2d::Vec3 Vec2to3(const cocos2d::Vec2 & cocosVec);
float AngleBToC(float angle);



class PlatformerGlobals{
public:
    static void drawNodeDebug(cocos2d::DrawNode * debugDrawer,const std::vector<cocos2d::Node *> & nodes);
    static void printVec2(const std::string & prefix,const cocos2d::Vec2 & vec);
    static void printMat4RowOrder(const std::string & prefix,const cocos2d::Mat4 & mat);

public:
};






#endif //MYGAME_PLATFORMERGLOBALS_HPP
