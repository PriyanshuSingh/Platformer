//
// Created by ashish on 4/12/16.
//

#ifndef MYGAME_PLATFORMERGLOBALS_HPP
#define MYGAME_PLATFORMERGLOBALS_HPP

#include <string>
#include <vector>
#define DEBUGGING_APP


namespace cocos2d {
    class Node;
    class DrawNode;
}

class PlatformerGlobals{
public:
    static void drawNodeDebug(cocos2d::DrawNode * debugDrawer,const std::vector<cocos2d::Node *> & nodes);


public:
    const static std::string MainPlayerName;


};






#endif //MYGAME_PLATFORMERGLOBALS_HPP
