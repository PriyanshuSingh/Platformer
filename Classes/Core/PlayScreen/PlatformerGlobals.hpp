//
// Created by ashish on 4/12/16.
//

#ifndef MYGAME_PLATFORMERGLOBALS_HPP
#define MYGAME_PLATFORMERGLOBALS_HPP

#include <string>
#include <vector>
#include "Box2D/Common/b2Math.h"


#define DEBUGGING_APP







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




class PlatformerGlobals{
public:
    static void drawNodeDebug(cocos2d::DrawNode * debugDrawer,const std::vector<cocos2d::Node *> & nodes);
    static void printVec2(const std::string & prefix,const cocos2d::Vec2 & vec);
    static void printMat4RowOrder(const std::string & prefix,const cocos2d::Mat4 & mat);

public:
    const static std::string MainPlayerName;


};






#endif //MYGAME_PLATFORMERGLOBALS_HPP
